LIVEDISPLAY_TARGETS := msm8916 msm8939 msm8974 msm8992 msm8994
LIVEDISPLAY_TARGETS += msm8996 msm8937 msm8953 msm8976

ifeq ($(call is-board-platform-in-list, $(LIVEDISPLAY_TARGETS)),true)

LOCAL_PATH:= $(call my-dir)

common_C_INCLUDES := \
	$(LOCAL_PATH)/impl \
    $(LOCAL_PATH)/inc

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(common_C_INCLUDES)

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libutils

LOCAL_SRC_FILES := \
    src/LiveDisplay.cpp \
    impl/Utils.cpp \
    impl/LegacyMM.cpp \
    impl/SDM.cpp

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := liblivedisplay
LOCAL_CFLAGS := -std=c++11

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_SRC_FILES	:= \
    jni/org_cyanogenmod_hardware_LiveDisplayVendorImpl.cpp

LOCAL_C_INCLUDES := $(common_C_INCLUDES)

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libnativehelper \
    libutils

LOCAL_STATIC_LIBRARIES := liblivedisplay

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libjni_livedisplay
LOCAL_CFLAGS := -std=c++11

include $(BUILD_SHARED_LIBRARY)

endif
