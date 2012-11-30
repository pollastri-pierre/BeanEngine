LOCAL_PATH := $(call my-dir)
MY_PATH := $(LOCAL_PATH)
 include $(call all-subdir-makefiles)

 include $(CLEAR_VARS)

 LOCAL_PATH := $(MY_PATH)
 
include $(CLEAR_VARS)

LOCAL_MODULE    := unicode

LOCAL_SRC_FILES	:=	utf8.c		\
					regress.c	\

include $(BUILD_STATIC_LIBRARY)