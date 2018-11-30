LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#will need to change this if you want compile
OPENCV_ROOT:=C:/androidOpenCV/OpenCV-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCV_ROOT}/sdk/native/jni/OpenCV.mk

NDK_MODULE_PATH=C:/AndroidSDK/ndk-bundle
LOCAL_ARM_NEON := true
LOCAL_SRC_FILES := com_example_albertReed_asuforiacppsupport_OpencvNativeClass.cpp
LOCAL_CPPFLAGS := -std=gnu++0x
LOCAL_CFLAGS += -O2
LOCAL_LDLIBS += -llog -ldl
LOCAL_MODULE := MyOpencvLibs

include $(BUILD_SHARED_LIBRARY)

