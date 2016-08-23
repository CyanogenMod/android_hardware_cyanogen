/*
** Copyright 2016, The CyanogenMod Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#define LOG_TAG "LiveDisplay-HW"

#include "jniutils.h"

#include "Types.h"
#include "LiveDisplay.h"

namespace android {

static struct {
    jclass clazz;
    jmethodID constructor;
    jfieldID id;
    jfieldID name;
} gDisplayModeClass;

static struct {
    jclass clazz;
    jmethodID constructor;
    jfieldID mHue;
    jfieldID mSaturation;
    jfieldID mIntensity;
    jfieldID mContrast;
    jfieldID mSaturationThreshold;
} gHSICClass;

static struct {
    jclass clazz;
    jmethodID constructor;
} gRangeClass;

static struct {
    jclass clazz;
    jmethodID constructor;
} gFloatClass;

static struct {
    jclass clazz;
    jmethodID constructor;
} gIntegerClass;


static jobject displayModeToObject(JNIEnv* env, sp<DisplayMode> mode)
{
    if (!mode.get() || mode->id < 0) {
        return NULL;
    }

    return env->NewObject(gDisplayModeClass.clazz, gDisplayModeClass.constructor,
            (jint) mode->id,
            env->NewStringUTF(mode->name.string()));
}

static sp<DisplayMode> objectToDisplayMode(JNIEnv* env, jobject mode)
{
    jstring name = (jstring) env->GetObjectField(mode, gDisplayModeClass.name);
    return new DisplayMode((int32_t) env->GetIntField(mode, gDisplayModeClass.id),
            env->GetStringUTFChars(name, NULL),
            env->GetStringUTFLength(name));
}

static jobject floatRangeToObject(JNIEnv* env, FloatRange range)
{
    jobject lower = env->NewObject(gFloatClass.clazz, gFloatClass.constructor,
            (jfloat) range.min);

    jobject upper = env->NewObject(gFloatClass.clazz, gFloatClass.constructor,
            (jfloat) range.max);

    return env->NewObject(gRangeClass.clazz, gRangeClass.constructor,
            lower, upper);
}

static jobject intRangeToObject(JNIEnv* env, Range range)
{
    jobject lower = env->NewObject(gIntegerClass.clazz, gIntegerClass.constructor,
            (jint) range.min);

    jobject upper = env->NewObject(gIntegerClass.clazz, gIntegerClass.constructor,
            (jint) range.max);

    return env->NewObject(gRangeClass.clazz, gRangeClass.constructor,
            lower, upper);
}

static jobject hsicToObject(JNIEnv* env, HSIC hsic)
{
    return env->NewObject(gHSICClass.clazz, gHSICClass.constructor,
            (jfloat) hsic.hue, (jfloat) hsic.saturation,
            (jfloat) hsic.intensity, (jfloat) hsic.contrast,
            (jfloat) hsic.saturationThreshold);
}

static jint org_cyanogenmod_hardware_LiveDisplayVendorImpl_getSupportedFeatures(
        JNIEnv* env __unused, jclass thiz __unused)
{
    return (jint) LiveDisplay::getInstance().getSupportedFeatures();
}

static jboolean org_cyanogenmod_hardware_LiveDisplayVendorImpl_isAdaptiveBacklightEnabled(
        JNIEnv* env __unused, jclass thiz __unused)
{
    return LiveDisplay::getInstance().isAdaptiveBacklightEnabled();
}

static jboolean org_cyanogenmod_hardware_LiveDisplayVendorImpl_setAdaptiveBacklightEnabled(
        JNIEnv* env __unused, jclass thiz __unused, jboolean enabled)
{
    return LiveDisplay::getInstance().setAdaptiveBacklightEnabled(enabled) == OK;
}

static jobjectArray org_cyanogenmod_hardware_LiveDisplayVendorImpl_getDisplayModes(
        JNIEnv* env, jclass thiz __unused)
{
    List<sp<DisplayMode>> modes;
    status_t rc = LiveDisplay::getInstance().getDisplayModes(modes);
    if (rc != OK) {
        return NULL;
    }

    jobjectArray modeList = env->NewObjectArray(modes.size(), gDisplayModeClass.clazz, NULL);
    int i = 0;

    if (rc == OK) {
        for (List<sp<DisplayMode>>::iterator it = modes.begin();
                it != modes.end(); ++it) {
            const sp<DisplayMode> mode = *it;
            env->SetObjectArrayElement(modeList, i,
                    displayModeToObject(env, mode));
            i++;
        }
    }

    return modeList;
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getCurrentDisplayMode(
        JNIEnv* env, jclass thiz __unused)
{
    return displayModeToObject(env,
            LiveDisplay::getInstance().getCurrentDisplayMode());
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getDefaultDisplayMode(
        JNIEnv* env, jclass thiz __unused)
{
    return displayModeToObject(env,
            LiveDisplay::getInstance().getDefaultDisplayMode());
}

static jboolean org_cyanogenmod_hardware_LiveDisplayVendorImpl_setDisplayMode(
        JNIEnv* env, jclass thiz __unused, jobject mode, jboolean makeDefault)
{
    return LiveDisplay::getInstance().setDisplayMode(
            objectToDisplayMode(env, mode)->id, makeDefault) == OK;
}

static jboolean org_cyanogenmod_hardware_LiveDisplayVendorImpl_isOutdoorModeEnabled(
        JNIEnv* env __unused, jclass thiz __unused)
{
    return LiveDisplay::getInstance().isOutdoorModeEnabled();
}

static jboolean org_cyanogenmod_hardware_LiveDisplayVendorImpl_setOutdoorModeEnabled(
        JNIEnv* env __unused, jclass thiz __unused, jboolean enabled)
{
    return LiveDisplay::getInstance().setOutdoorModeEnabled(enabled) == OK;
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getColorBalanceRange(
        JNIEnv* env __unused, jclass thiz __unused)
{
    Range range;
    if (LiveDisplay::getInstance().getColorBalanceRange(range) == OK) {
        return intRangeToObject(env, range);
    }
    return NULL;
}

static jint org_cyanogenmod_hardware_LiveDisplayVendorImpl_getColorBalance(
        JNIEnv* env __unused, jclass thiz __unused)
{
    return LiveDisplay::getInstance().getColorBalance();
}

static jboolean org_cyanogenmod_hardware_LiveDisplayVendorImpl_setColorBalance(
        JNIEnv* env __unused, jclass thiz __unused, jint value)
{
    return LiveDisplay::getInstance().setColorBalance(value) == OK;
}

static jboolean org_cyanogenmod_hardware_LiveDisplayVendorImpl_setPictureAdjustment(
        JNIEnv* env __unused, jclass thiz __unused, jobject hsicObj)
{
    HSIC hsic(static_cast<int32_t>(env->GetFloatField(hsicObj, gHSICClass.mHue)),
            env->GetFloatField(hsicObj, gHSICClass.mSaturation),
            env->GetFloatField(hsicObj, gHSICClass.mIntensity),
            env->GetFloatField(hsicObj, gHSICClass.mContrast),
            env->GetFloatField(hsicObj, gHSICClass.mSaturationThreshold));

    return LiveDisplay::getInstance().setPictureAdjustment(hsic) == OK;
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getPictureAdjustment(
        JNIEnv* env __unused, jclass thiz __unused)
{
    HSIC hsic;

    ALOGD("getPictureAdjustment");
    if (LiveDisplay::getInstance().getPictureAdjustment(hsic) != OK) {
        return NULL;
    }

    return hsicToObject(env, hsic);
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getDefaultPictureAdjustment(
        JNIEnv* env __unused, jclass thiz __unused)
{
    HSIC hsic;

    ALOGD("getDefaultPictureAdjustment");
    if (LiveDisplay::getInstance().getDefaultPictureAdjustment(hsic) != OK) {
        return NULL;
    }

    return hsicToObject(env, hsic);
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getPictureAdjustmentRanges(
        JNIEnv* env __unused, jclass thiz __unused)
{
    HSICRanges ranges;
    if (LiveDisplay::getInstance().getPictureAdjustmentRanges(ranges) == OK) {
        jobjectArray modeList = env->NewObjectArray(5, gRangeClass.clazz, NULL);
        env->SetObjectArrayElement(modeList, 0, intRangeToObject(env, ranges.hue));
        env->SetObjectArrayElement(modeList, 1, floatRangeToObject(env, ranges.saturation));
        env->SetObjectArrayElement(modeList, 2, floatRangeToObject(env, ranges.intensity));
        env->SetObjectArrayElement(modeList, 3, floatRangeToObject(env, ranges.contrast));
        env->SetObjectArrayElement(modeList, 4, floatRangeToObject(env, ranges.saturationThreshold));
        return modeList;
    }
    return NULL;

}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getHueRange(
        JNIEnv* env __unused, jclass thiz __unused)
{
    HSICRanges ranges;
    if (LiveDisplay::getInstance().getPictureAdjustmentRanges(ranges) == OK) {
        return floatRangeToObject(env, FloatRange(ranges.hue.min, ranges.hue.max));
    }
    return NULL;
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getSaturationRange(
        JNIEnv* env __unused, jclass thiz __unused)
{
    HSICRanges ranges;
    if (LiveDisplay::getInstance().getPictureAdjustmentRanges(ranges) == OK) {
        return floatRangeToObject(env, ranges.saturation);
    }
    return NULL;
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getIntensityRange(
        JNIEnv* env __unused, jclass thiz __unused)
{
    HSICRanges ranges;
    if (LiveDisplay::getInstance().getPictureAdjustmentRanges(ranges) == OK) {
        return floatRangeToObject(env, ranges.intensity);
    }
    return NULL;
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getContrastRange(
        JNIEnv* env __unused, jclass thiz __unused)
{
    HSICRanges ranges;
    if (LiveDisplay::getInstance().getPictureAdjustmentRanges(ranges) == OK) {
        return floatRangeToObject(env, ranges.contrast);
    }
    return NULL;
}

static jobject org_cyanogenmod_hardware_LiveDisplayVendorImpl_getSaturationThresholdRange (
        JNIEnv* env __unused, jclass thiz __unused)
{
    HSICRanges ranges;
    if (LiveDisplay::getInstance().getPictureAdjustmentRanges(ranges) == OK) {
        return floatRangeToObject(env, ranges.saturationThreshold);
    }
    return NULL;
}


static JNINativeMethod gLiveDisplayVendorImplMethods[] = {
    { "native_getSupportedFeatures",
        "()I",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getSupportedFeatures },
    { "native_isAdaptiveBacklightEnabled",
        "()Z",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_isAdaptiveBacklightEnabled },
    { "native_setAdaptiveBacklightEnabled",
        "(Z)Z",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_setAdaptiveBacklightEnabled },
    { "native_getDisplayModes",
        "()[Lcyanogenmod/hardware/DisplayMode;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getDisplayModes },
    { "native_getCurrentDisplayMode",
        "()Lcyanogenmod/hardware/DisplayMode;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getCurrentDisplayMode },
    { "native_getDefaultDisplayMode",
        "()Lcyanogenmod/hardware/DisplayMode;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getDefaultDisplayMode },
    { "native_setDisplayMode",
        "(Lcyanogenmod/hardware/DisplayMode;Z)Z",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_setDisplayMode },
    { "native_isOutdoorModeEnabled",
        "()Z",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_isOutdoorModeEnabled },
    { "native_setOutdoorModeEnabled",
        "(Z)Z",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_setOutdoorModeEnabled },
    { "native_getColorBalanceRange",
        "()Landroid/util/Range;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getColorBalanceRange },
    { "native_getColorBalance",
        "()I",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getColorBalance },
    { "native_setColorBalance",
        "(I)Z",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_setColorBalance },
    { "native_setPictureAdjustment",
        "(Lcyanogenmod/hardware/HSIC;)Z",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_setPictureAdjustment },
    { "native_getPictureAdjustment",
        "()Lcyanogenmod/hardware/HSIC;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getPictureAdjustment },
    { "native_getDefaultPictureAdjustment",
        "()Lcyanogenmod/hardware/HSIC;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getDefaultPictureAdjustment },
    { "native_getHueRange",
        "()Landroid/util/Range;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getHueRange },
    { "native_getSaturationRange",
        "()Landroid/util/Range;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getSaturationRange },
    { "native_getIntensityRange",
        "()Landroid/util/Range;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getIntensityRange },
    { "native_getContrastRange",
        "()Landroid/util/Range;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getContrastRange },
    { "native_getSaturationThresholdRange",
        "()Landroid/util/Range;",
        (void *)org_cyanogenmod_hardware_LiveDisplayVendorImpl_getSaturationThresholdRange },
};


static int register_org_cyanogenmod_hardware_LiveDisplayVendorImpl(JNIEnv *env)
{
    FIND_CLASS(gDisplayModeClass.clazz,
            "cyanogenmod/hardware/DisplayMode");
    GET_METHOD_ID(gDisplayModeClass.constructor,
            gDisplayModeClass.clazz, "<init>", "(ILjava/lang/String;)V");
    GET_FIELD_ID(gDisplayModeClass.id,
            gDisplayModeClass.clazz, "id", "I");
    GET_FIELD_ID(gDisplayModeClass.name,
            gDisplayModeClass.clazz, "name", "Ljava/lang/String;");

    FIND_CLASS(gHSICClass.clazz,
            "cyanogenmod/hardware/HSIC");
    GET_METHOD_ID(gHSICClass.constructor,
            gHSICClass.clazz, "<init>", "(FFFFF)V");
    GET_FIELD_ID(gHSICClass.mHue,
            gHSICClass.clazz, "mHue", "F");
    GET_FIELD_ID(gHSICClass.mSaturation,
            gHSICClass.clazz, "mSaturation", "F");
    GET_FIELD_ID(gHSICClass.mIntensity,
            gHSICClass.clazz, "mIntensity", "F");
    GET_FIELD_ID(gHSICClass.mContrast,
            gHSICClass.clazz, "mContrast", "F");
    GET_FIELD_ID(gHSICClass.mSaturationThreshold,
            gHSICClass.clazz, "mSaturationThreshold", "F");

    FIND_CLASS(gRangeClass.clazz,
            "android/util/Range");
    GET_METHOD_ID(gRangeClass.constructor,
            gRangeClass.clazz, "<init>", "(Ljava/lang/Comparable;Ljava/lang/Comparable;)V");

    FIND_CLASS(gFloatClass.clazz, "java/lang/Float");
    GET_METHOD_ID(gFloatClass.constructor,
            gFloatClass.clazz, "<init>", "(F)V");

    FIND_CLASS(gIntegerClass.clazz, "java/lang/Integer");
    GET_METHOD_ID(gIntegerClass.constructor,
            gIntegerClass.clazz, "<init>", "(I)V");


    int rc = jniRegisterNativeMethods(env,
            "org/cyanogenmod/hardware/LiveDisplayVendorImpl",
            gLiveDisplayVendorImplMethods,
            NELEM(gLiveDisplayVendorImplMethods));
    ALOGD("LiveDisplay registerNatives = %d", rc);
    return rc;
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved __unused)
{
    JNIEnv* env = NULL;
    jint result = -1;

    ALOGD("LiveDisplay JNI ONLOAD");

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("GetEnv failed!");
        return result;
    }
    ALOG_ASSERT(env, "Could not retrieve the env!");

    register_org_cyanogenmod_hardware_LiveDisplayVendorImpl(env);

    return JNI_VERSION_1_4;
}

};
