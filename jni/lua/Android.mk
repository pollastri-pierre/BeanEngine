LOCAL_PATH := $(call my-dir)
MY_PATH := $(LOCAL_PATH)
 include $(call all-subdir-makefiles)

 include $(CLEAR_VARS)

 LOCAL_PATH := $(MY_PATH)
 
include $(CLEAR_VARS)
 
LOCAL_ARM_MODE  := arm
LOCAL_MODULE    :=  lua
LOCAL_LDLIBS	:= -llog
LOCAL_SRC_FILES := 	lapi.cpp \
					lauxlib.cpp \
					lbaselib.cpp \
					lcode.cpp \
					ldblib.cpp \
					ldebug.cpp \
					ldo.cpp \
					ldump.cpp \
					lfunc.cpp \
					lgc.cpp \
					linit.cpp \
					liolib.cpp \
					llex.cpp \
					lmathlib.cpp \
					lmem.cpp \
					loadlib.cpp \
					lobject.cpp \
					lopcodes.cpp \
					loslib.cpp \
					lparser.cpp \
					lstate.cpp \
					lstring.cpp \
					lstrlib.cpp \
					ltable.cpp \
					ltablib.cpp \
					ltm.cpp \
					lundump.cpp \
					lvm.cpp \
					lzio.cpp \
					print.cpp
 
include $(BUILD_STATIC_LIBRARY)