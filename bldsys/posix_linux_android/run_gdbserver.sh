 
#!/bin/bash

#
# Copyright (C) YuqiaoZhang(HanetakaYuminaga)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

MY_DIR="$(readlink -f "$(dirname "$0")")"
cd ${MY_DIR}

# https://developer.android.com/studio/debug#debug-types
# android-ndk-r14b/prebuilt/windows-x86_64/bin/ndk-gdb.py
# android-ndk-r14b/python-packages/gdbrunner/__init__.py
# gdbrunner.get_run_as_cmd

ADB_CMD="${MY_DIR}/android-sdk/platform-tools/adb"
PACKAGE_NAME=YuqiaoZhang.HanetakaYuminaga.PatriotEngine
LAUNCH_ACTIVITY_NAME=android.app.NativeActivity
ARCH=arm64
DELAY=0.25s
PORT=5039
OUT_DIR="${MY_DIR}/obj/debug/local/arm64-v8a"

DATA_DIR="$("${ADB_CMD}" shell "run-as "${PACKAGE_NAME}" sh -c 'pwd' 2>/dev/null" | xargs)"
if test -z ${DATA_DIR}; then
    echo "Could not find application's data directory. Are you sure that the application is installed and debuggable?"
    exit 1
fi

# Applications with minSdkVersion >= 24 will have their data directories
# created with rwx------ permissions, preventing adbd from forwarding to
# the gdbserver socket. To be safe, if we're on a device >= 24, always
# chmod the directory.
if "${ADB_CMD}" shell "run-as "${PACKAGE_NAME}" chmod 711 "${DATA_DIR}""; then # chmod a+x
    echo "Found application data directory: ${DATA_DIR}"
else
    echo "Failed to make application data directory world executable"
fi
APP_DATA_DIR="${DATA_DIR}"

APP_GDBSERVER_PATH="${APP_DATA_DIR}/${ARCH}-gdbserver"
if "${ADB_CMD}" shell "run-as "${PACKAGE_NAME}" ls "${APP_GDBSERVER_PATH}" 1>/dev/null 2>/dev/null"; then
    echo "Found app gdbserver: ${APP_GDBSERVER_PATH}"
else
    # We need to upload our gdbserver
    echo "App gdbserver not found at ${APP_GDBSERVER_PATH}, uploading."
    LOCAL_PATH="${MY_DIR}/android-ndk-r14b/prebuilt/android-${ARCH}/gdbserver/gdbserver"
    REMOTE_PATH="/data/local/tmp/${ARCH}-gdbserver"
    "${ADB_CMD}" push "${LOCAL_PATH}" "${REMOTE_PATH}"

    # Copy gdbserver into the data directory on M+, because selinux prevents
    # execution of binaries directly from /data/local/tmp.
    DESTINATION="${APP_GDBSERVER_PATH}"
    
    echo "Copying gdbserver to ${DESTINATION}."
    if "${ADB_CMD}" shell "run-as "${PACKAGE_NAME}" sh -c 'cat '${REMOTE_PATH}' | cat > '${DESTINATION}''"; then
        echo "Uploaded gdbserver to ${DESTINATION}"
    else
        echo "Failed to chmod gdbserver at ${DESTINATION}."
        exit 1
    fi

    # Make gdbserver executable
    if "${ADB_CMD}" shell "run-as "${PACKAGE_NAME}" chmod 711 "${APP_GDBSERVER_PATH}""; then
        echo "Made ${APP_GDBSERVER_PATH} executable"
    else
        echo "Failed to make ${APP_GDBSERVER_PATH} executable"
    fi
fi

if "${ADB_CMD}" shell "ls /system/bin/readlink 1>/dev/null 2>/dev/null"; then
    if test "$("${ADB_CMD}" shell "readlink /system/bin/ps")" = "toolbox"; then
        PS_SCRIPT="ps"
    else
        PS_SCRIPT="ps -w"
    fi
else
    PS_SCRIPT="ps"
fi

# Kill the process and gdbserver if requested.
KILL_PIDS=$("${ADB_CMD}" shell ${PS_SCRIPT} | grep "${APP_GDBSERVER_PATH}" | awk '{print $2}' | xargs) 
if test '!' '(' '-z' "${KILL_PIDS}" ')'; then
    if "${ADB_CMD}" shell "run-as "${PACKAGE_NAME}" kill ${KILL_PIDS}"; then # SIGKILL not support
        echo "Killed ${KILL_PIDS}"
    else
        echo "Failed to kill ${KILL_PIDS}"
        exit 1
    fi  
fi

KILL_PIDS=$("${ADB_CMD}" shell ${PS_SCRIPT} | grep "${PACKAGE_NAME}" | awk '{print $2}' | xargs) 
if test '!' '(' '-z' "${KILL_PIDS}" ')'; then
    if "${ADB_CMD}" shell "run-as "${PACKAGE_NAME}" kill ${KILL_PIDS}"; then # SIGKILL not support
        echo "Killed ${KILL_PIDS}"
    else
        echo "Failed to kill ${KILL_PIDS}"
        exit 1
    fi  
fi

# Launch the application if needed, and get its pid
COMPONENT_NAME="${PACKAGE_NAME}/${LAUNCH_ACTIVITY_NAME}"
if "${ADB_CMD}" shell "am start ${COMPONENT_NAME}"; then # -D # wait java
    echo "Launching activity ${COMPONENT_NAME}..."
else
    echo "Failed to start ${COMPONENT_NAME}"
    exit 1
fi 

sleep ${DELAY} # wait the activity to launch

PIDS=$("${ADB_CMD}" shell ${PS_SCRIPT} | grep "${PACKAGE_NAME}" | awk '{print $2}' | xargs) 
LEN_PIDS=0
for PID in ${PIDS}
do
   LEN_PIDS=$((LEN_PIDS+1))
done

if test 0 -eq ${LEN_PIDS}; then
    echo "Failed to find running process "${PACKAGE_NAME}"" # you may increase the delay
    exit 1
fi

if test 1 -lt ${LEN_PIDS}; then
    echo "Multiple running processes named "${PACKAGE_NAME}""
    exit 1
fi
PID=${PIDS}

# Pull the linker, zygote, and notable system libraries
mkdir -p "${OUT_DIR}/system/bin"
mkdir -p "${OUT_DIR}/system/lib64"
REQUIRED_FILES="/system/bin/app_process64 /system/lib64/libc.so /system/lib64/libm.so /system/lib64/libdl.so"
for REQUIRED_FILE in ${REQUIRED_FILES}
do
    LOCAL_PATH="${OUT_DIR}/${REQUIRED_FILE}"
    echo "Pulling "${REQUIRED_FILE}" to "${LOCAL_PATH}""
    "${ADB_CMD}" pull "${REQUIRED_FILE}" "${LOCAL_PATH}"
done

# Start gdbserver.
DEBUG_SOCKET="${APP_DATA_DIR}/debug_socket"
echo "Starting gdbserver..."
#
# Start gdbserver in the background and forward necessary ports.
#
# Args:
#   device: ADB device to start gdbserver on.
#   gdbserver_local_path: Host path to push gdbserver from, can be None.
#   gdbserver_remote_path: Device path to push gdbserver to.
#   target_pid: PID of device process to attach to.
#   run_cmd: Command to run on the device.
#   debug_socket: Device path to place gdbserver unix domain socket.
#   port: Host port to forward the debug_socket to.
#   user: Device user to run gdbserver as.
#
#   Returns:
#       Popen handle to the `adb shell` process gdbserver was started with.
#
GDBSERVER_REMOTE_PATH=${APP_GDBSERVER_PATH}
TARGET_PID=${PID}

# ${GDBSERVER_REMOTE_PATH }--once
"${ADB_CMD}" forward "tcp:${PORT}" "localfilesystem:${DEBUG_SOCKET}"
trap ""${ADB_CMD}" forward --remove "tcp:${PORT}"" EXIT

"${ADB_CMD}" shell "run-as "${PACKAGE_NAME}" ${GDBSERVER_REMOTE_PATH} --once +"${DEBUG_SOCKET}" --attach ${TARGET_PID}"
