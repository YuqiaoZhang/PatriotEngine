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

LOCAL_PATH:= $(call my-dir)

# examples / general_acyclic_graphs_of_tasks

include $(CLEAR_VARS)

LOCAL_MODULE := pt_general_acyclic_graphs_of_tasks

LOCAL_SRC_FILES:= \
	$(abspath $(LOCAL_PATH)/../../examples/mcrt/general_acyclic_graphs_of_tasks)/main.cpp \

#LOCAL_CFLAGS += -fdiagnostics-format=msvc
LOCAL_CFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8
LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_CPPFLAGS += -std=c++11

LOCAL_LDFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8
LOCAL_LDFLAGS += -Wl,--enable-new-dtags # the linker can't recognize the old dtags
LOCAL_LDFLAGS += -Wl,-rpath,XORIGIN # chrpath can only make path shorter
LOCAL_LDFLAGS += -Wl,--version-script,$(abspath $(LOCAL_PATH))/pt_general_acyclic_graphs_of_tasks.def

LOCAL_SHARED_LIBRARIES := libpt_mcrt

include $(BUILD_EXECUTABLE)
