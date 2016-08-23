LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := pp_client
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_SRC_FILES := pp_client.c
include $(BUILD_EXECUTABLE)

