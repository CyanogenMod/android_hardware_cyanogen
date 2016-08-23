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

#include <cutils/sockets.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LOG_TAG "LiveDisplay-LegacyMM"
#include <utils/Log.h>

#include "LegacyMM.h"

namespace android {

status_t LegacyMM::initialize() {
    status_t rc = OK;
    mLibHandle = dlopen(MM_DISP_LIB, RTLD_NOW);
    if (mLibHandle == NULL) {
        ALOGE("DLOPEN failed for %s", MM_DISP_LIB);
        return NO_INIT;
    }

    disp_api_init = (int(*)(int32_t))dlsym(mLibHandle, "disp_api_init");
    if (disp_api_init == NULL) {
        ALOGE("dlsym failed for disp_api_init");
    }
    disp_api_supported = (int(*)(int32_t, int32_t))dlsym(mLibHandle, "disp_api_supported");
    if (disp_api_supported == NULL) {
        ALOGE("dlsym failed for disp_api_supported");
    }
    disp_api_get_color_balance_range =
        (int(*)(int32_t, void*))dlsym(mLibHandle, "disp_api_get_color_balance_range");
    if (disp_api_get_color_balance_range == NULL) {
        ALOGE("dlsym failed for disp_api_get_color_balance_range");
    }
    disp_api_set_color_balance =
        (int(*)(int32_t, int))dlsym(mLibHandle, "disp_api_set_color_balance");
    if (disp_api_set_color_balance == NULL) {
        ALOGE("dlsym failed for disp_api_set_color_balance");
    }
    disp_api_get_color_balance =
        (int(*)(int32_t, int*))dlsym(mLibHandle, "disp_api_get_color_balance");
    if (disp_api_get_color_balance == NULL) {
        ALOGE("dlsym failed for disp_api_get_color_balance");
    }
    disp_api_get_num_display_modes =
        (int(*)(int32_t, int32_t, int*))dlsym(mLibHandle, "disp_api_get_num_display_modes");
    if (disp_api_get_num_display_modes == NULL) {
        ALOGE("dlsym failed for disp_api_get_num_display_modes");
    }
    disp_api_get_display_modes =(int(*)(int32_t, int32_t, void*, int))dlsym(
        mLibHandle, "disp_api_get_display_modes");
    if (disp_api_get_display_modes == NULL) {
        ALOGE("dlsym failed for disp_api_get_display_modes");
    }
    disp_api_get_active_display_mode = (int(*)(int32_t, int*, uint32_t*))dlsym(
        mLibHandle, "disp_api_get_active_display_mode");
    if (disp_api_get_active_display_mode == NULL) {
        ALOGE("dlsym failed for disp_api_get_active_display_mode");
    }
    disp_api_set_active_display_mode =
        (int(*)(int32_t, int))dlsym(mLibHandle, "disp_api_set_active_display_mode");
    if (disp_api_set_active_display_mode == NULL) {
        ALOGE("dlsym failed for disp_api_set_active_display_mode");
    }
    disp_api_set_default_display_mode =
        (int(*)(int32_t, int))dlsym(mLibHandle, "disp_api_set_default_display_mode");
    if (disp_api_set_default_display_mode == NULL) {
        ALOGE("dlsym failed for disp_api_set_default_display_mode");
    }
    disp_api_get_default_display_mode =
        (int(*)(int32_t, int*))dlsym(mLibHandle, "disp_api_get_default_display_mode");
    if (disp_api_get_default_display_mode == NULL) {
        ALOGE("dlsym failed for disp_api_get_default_display_mode");
    }
    disp_api_get_pa_range = (int(*)(int32_t, void*))dlsym(mLibHandle, "disp_api_get_pa_range");
    if (disp_api_get_pa_range == NULL) {
        ALOGE("dlsym failed for disp_api_get_pa_range");
    }
    disp_api_get_pa_config =
        (int(*)(int32_t, void*))dlsym(mLibHandle, "disp_api_get_pa_config");
    if (disp_api_get_pa_config == NULL) {
        ALOGE("dlsym failed for disp_api_get_pa_config");
    }
    disp_api_set_pa_config =
        (int(*)(int32_t, void*))dlsym(mLibHandle, "disp_api_set_pa_config");
    if (disp_api_set_pa_config == NULL) {
        ALOGE("dlsym failed for disp_api_set_pa_config");
    }

    return disp_api_init(0);
}

LegacyMM::~LegacyMM() {
    if (mLibHandle != NULL) {
        dlclose(mLibHandle);
    }
}

status_t LegacyMM::deinitialize() {
    if (mLibHandle != NULL) {
        disp_api_init(1);
    }
    return OK;
}

bool LegacyMM::hasFeature(Feature feature) {
    int id;
    switch (feature) {
        case Feature::COLOR_TEMPERATURE:
            id = 0;
            break;
        case Feature::DISPLAY_MODES:
            id = 1;
            break;
        case Feature::PICTURE_ADJUSTMENT:
            id = 4;
            break;
        default:
            return false;
    }
    if (disp_api_supported(0, id)) {
        // display modes and color balance depend on each other
        if (feature == Feature::DISPLAY_MODES ||
                feature == Feature::COLOR_TEMPERATURE) {
            if (getNumDisplayModes() > 0) {
                // make sure the range isn't zero
                if (feature == Feature::COLOR_TEMPERATURE) {
                    Range r;
                    if (getColorBalanceRange(r) == OK && r.isNonZero()) {
                        return true;
                    }
                    return false;
                }
                return true;
            }
        }
        if (feature == Feature::PICTURE_ADJUSTMENT) {
            HSICRanges r;
            if (getPictureAdjustmentRanges(r) == OK && r.isValid()) {
                return true;
            }
        }
    }
    return false;
}

status_t LegacyMM::getColorBalanceRange(Range& range) {
    struct mm_range r;
    memset(&r, 0, sizeof(struct mm_range));

    status_t rc = disp_api_get_color_balance_range(0, &r);
    if (rc == OK) {
        range.min = r.min;
        range.max = r.max;
    }
    return rc;
}

status_t LegacyMM::setColorBalance(int32_t balance) {
    return disp_api_set_color_balance(0, (int)balance);
}

int32_t LegacyMM::getColorBalance() {
    int value = 0;
    if (disp_api_get_color_balance(0, &value) != 0) {
        value = 0;
    }
    return (int32_t)value;
}

int LegacyMM::getNumDisplayModes() {
    int count = 0;
    if (disp_api_get_num_display_modes(0, 0, &count) != 0) {
        count = 0;
    }
    return count;
}

status_t LegacyMM::getDisplayModes(List<sp<DisplayMode>>& profiles) {
    status_t rc = OK;
    int i = 0;

    int count = getNumDisplayModes();

    if (!count) return rc;

    struct d_mode {
        int id;
        char* name;
        uint32_t len;
        int32_t type;
    };

    d_mode* tmp = new d_mode[count];
    memset(tmp, 0, sizeof(d_mode) * count);
    for (i = 0; i < count; i++) {
        tmp[i].id = -1;
        tmp[i].name = new char[128];
        tmp[i].len = 128;
    }

    rc = disp_api_get_display_modes(0, 0, tmp, count);
    if (rc == 0) {
        for (i = 0; i < count; i++) {
            const sp<DisplayMode> m = new DisplayMode(tmp[i].id, tmp[i].name, tmp[i].len);
            profiles.push_back(m);
            delete tmp[i].name;
        }
    }
    delete[] tmp;

    return rc;
}

status_t LegacyMM::setDisplayMode(int32_t modeID, bool makeDefault) {
    if (disp_api_set_active_display_mode(0, modeID) != 0) {
        return BAD_VALUE;
    }

    if (makeDefault && disp_api_set_default_display_mode(0, modeID) != 0) {
        return BAD_VALUE;
    }

    return OK;
}

sp<DisplayMode> LegacyMM::getDisplayModeById(int id) {
    List<sp<DisplayMode>> profiles;
    status_t rc = getDisplayModes(profiles);
    if (rc == OK) {
        for (List<sp<DisplayMode>>::iterator it = profiles.begin(); it != profiles.end(); ++it) {
            const sp<DisplayMode> mode = *it;
            if (id == mode->id) {
                return mode;
            }
        }
    }

    return nullptr;
}

sp<DisplayMode> LegacyMM::getCurrentDisplayMode() {
    int id = 0;
    uint32_t mask = 0;

    status_t rc = disp_api_get_active_display_mode(0, &id, &mask);
    if (rc == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return nullptr;
}

sp<DisplayMode> LegacyMM::getDefaultDisplayMode() {
    int id = 0;

    status_t rc = disp_api_get_default_display_mode(0, &id);
    if (rc == OK && id >= 0) {
        return getDisplayModeById(id);
    }

    return nullptr;
}

status_t LegacyMM::getPictureAdjustmentRanges(HSICRanges& ranges) {
    struct mm_pa_range r;
    memset(&r, 0, sizeof(struct mm_pa_range));

    status_t rc = disp_api_get_pa_range(0, &r);
    if (rc == OK) {
        ranges.hue.min = r.min.hue;
        ranges.hue.max = r.max.hue;
        ranges.saturation.min = r.min.saturation;
        ranges.saturation.max = r.max.saturation;
        ranges.intensity.min = r.min.intensity;
        ranges.intensity.max = r.max.intensity;
        ranges.contrast.min = r.min.contrast;
        ranges.contrast.max = r.max.contrast;
        ranges.saturationThreshold.min = r.min.saturationThreshold;
        ranges.saturationThreshold.max = r.max.saturationThreshold;
    }
    return rc;
}

status_t LegacyMM::getPictureAdjustment(HSIC& hsic) {
    struct mm_pa_config config;
    memset(&config, 0, sizeof(struct mm_pa_config));

    status_t rc = disp_api_get_pa_config(0, &config);
    if (rc == OK) {
        hsic.hue = config.data.hue;
        hsic.saturation = config.data.saturation;
        hsic.intensity = config.data.intensity;
        hsic.contrast = config.data.contrast;
        hsic.saturationThreshold = config.data.saturationThreshold;
    }
    return rc;
}

status_t LegacyMM::getDefaultPictureAdjustment(HSIC& /* hsic */) {
    return OK;
}

status_t LegacyMM::setPictureAdjustment(HSIC hsic) {
    struct mm_pa_config config;
    memset(&config, 0, sizeof(struct mm_pa_config));

    config.flags = 0x0F;   // lower 4 bits
    config.data.hue = hsic.hue;
    config.data.saturation = hsic.saturation;
    config.data.intensity = hsic.intensity;
    config.data.contrast = hsic.contrast;
    config.data.saturationThreshold = hsic.saturationThreshold;

    return disp_api_set_pa_config(0, &config);
}
};
