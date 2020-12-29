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

# configure
if test \( $# -ne 3 \);
then
    echo "Usage: build.sh config platform arch"
    echo ""
    echo "Configs:"
    echo "  debug   -   build with the debug configuration"
    echo "  release -   build with the release configuration"
    echo ""
    echo "Platforms:"
    echo "  bionic  -   build with the bionic platform"
    echo "  glibc   -   build with the glibc platform"
    echo ""
    echo "Archs:"
    echo "  x86     -   build with the x86 arch"
    echo "  x64     -   build with the x64 arch"
    echo ""
    exit 1
fi

if test \( \( -n "$1" \) -a \( "$1" = "debug" \) \);then 
    NDK_BUILD_ARG_CONFIG="APP_DEBUG:=true"
    INT_DIR_CONFIG="obj/local"
    OUT_DIR_CONFIG="debug"
elif test \( \( -n "$1" \) -a \( "$1" = "release" \) \);then
    NDK_BUILD_ARG_CONFIG="APP_DEBUG:=false"
    INT_DIR_CONFIG="libs"
    OUT_DIR_CONFIG="release"
else
    echo "The config \"$1\" is not supported!"
    echo ""
    echo "Configs:"
    echo "  debug   -   build with the debug configuration"
    echo "  release -   build with the release configuration"
    echo ""
    exit 1
fi

if test \( \( -n "$2" \) -a \( "$2" = "bionic" \) \);then
    NDK_BUILD_ARG_PLATFORM="APP_BUILD_SCRIPT:=build_bionic.mk"
    NDK_BUILD_CMD_DIR_PLATFORM="android-ndk-r14b"
elif test \( \( -n "$2" \) -a \( "$2" = "glibc" \) \);then
    NDK_BUILD_ARG_PLATFORM="APP_BUILD_SCRIPT:=build_glibc.mk"
    NDK_BUILD_CMD_DIR_PLATFORM="ndk_build_glibc"
else
    echo "The platform \"$2\" is not supported!"
    echo ""
    echo "Platforms:"
    echo "  bionic  -   build with the bionic platform"
    echo "  glibc   -   build with the glibc platform"
    echo ""
    exit 1
fi

if test \( \( -n "$3" \) -a \( "$3" = "x86" \) \);then
    NDK_BUILD_ARG_ARCH="APP_ABI:=x86"
    INT_DIR_ARCH="x86"
    OUT_DIR_ARCH="x86"
elif test \( \( -n "$3" \) -a \( "$3" = "x64" \) \);then
    NDK_BUILD_ARG_ARCH="APP_ABI:=x86_64"
    INT_DIR_ARCH="x86_64"
    OUT_DIR_ARCH="x64"
else
    echo "The architecture \"$3\" is not supported!"
    echo ""
    echo "Archs:"
    echo "  x86     -   build with the x86 arch"
    echo "  x64     -   build with the x64 arch"
    echo ""
    exit 1
fi

MY_DIR="$(readlink -f "$(dirname "$0")")"

NDK_BUILD_CMD_DIR="$(realpath -m "${MY_DIR}/${NDK_BUILD_CMD_DIR_PLATFORM}/")"
NDK_BUILD_ARGS="${NDK_BUILD_ARG_CONFIG} ${NDK_BUILD_ARG_ARCH} NDK_PROJECT_PATH:=null NDK_OUT:=obj NDK_LIBS_OUT:=libs NDK_APPLICATION_MK:=Application.mk ${NDK_BUILD_ARG_PLATFORM}"

INT_DIR="$(realpath -m "${MY_DIR}/${INT_DIR_CONFIG}/${INT_DIR_ARCH}/")"
OUT_DIR="$(realpath -m "${MY_DIR}/../../bin/posix_linux_x11/${OUT_DIR_ARCH}/${OUT_DIR_CONFIG}/")"

OUT_BINS="libpt_mcrt.so libpt_tbbmalloc.so libpt_irml.so libpt_tbb.so libpt_gfx.so pt_launcher libpt_gfx_wsi_bitmap.so pt_general_acyclic_graphs_of_tasks"

# build by ndk  
cd ${MY_DIR}

# rm -rf obj
# rm -rf libs

if ${NDK_BUILD_CMD_DIR}/ndk-build ${NDK_BUILD_ARGS}; then #V=1 VERBOSE=1 
    echo "ndk-build passed"
else
    echo "ndk-build failed"
    exit 1
fi

# before execute change the rpath to \$ORIGIN    
# fix me: define the $ORIGIN correctly in the Linux_X11.mk
for i in ${OUT_BINS}
do
    chrpath -r '$ORIGIN' ${INT_DIR}/${i} 
done

# mkdir the out dir if necessary
mkdir -p ${OUT_DIR}

# copy the unstriped so to out dir
for i in ${OUT_BINS}
do
    rm -rf ${OUT_DIR}/${i}
    cp -f ${INT_DIR}/${i} ${OUT_DIR}/
done

