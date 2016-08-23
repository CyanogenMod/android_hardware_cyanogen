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

#ifndef CYNGN_LIVEDISPLAYLEGACYMM_H
#define CYNGN_LIVEDISPLAYLEGACYMM_H

#include <LiveDisplayBackend.h>

#define MM_DISP_LIB "libmm-disp-apis.so"

namespace android {

struct mm_pa_data {
    int hue;
    int saturation;
    int intensity;
    int contrast;
    int saturationThreshold;
};

struct mm_pa_config {
    int flags;
    struct mm_pa_data data;
};

struct mm_pa_range {
    struct mm_pa_data max;
    struct mm_pa_data min;
};

struct mm_range {
    int max;
    int min;
};

class LegacyMM : public LiveDisplayBackend {
  public:
    virtual status_t initialize();
    virtual status_t deinitialize();
    virtual bool hasFeature(Feature feature);

    virtual status_t setAdaptiveBacklightEnabled(bool /* enabled */) {
        return NO_INIT;
    }
    virtual bool isAdaptiveBacklightEnabled() {
        return false;
    }

    virtual status_t setOutdoorModeEnabled(bool /* enabled */) {
        return NO_INIT;
    }
    virtual bool isOutdoorModeEnabled() {
        return false;
    }

    virtual status_t getColorBalanceRange(Range& range);
    virtual status_t setColorBalance(int32_t balance);
    virtual int32_t getColorBalance();

    virtual status_t getDisplayModes(List<sp<DisplayMode>>& profiles);
    virtual status_t setDisplayMode(int32_t modeID, bool makeDefault);
    virtual sp<DisplayMode> getCurrentDisplayMode();
    virtual sp<DisplayMode> getDefaultDisplayMode();

    virtual status_t getPictureAdjustmentRanges(HSICRanges& ranges);
    virtual status_t getPictureAdjustment(HSIC& hsic);
    virtual status_t getDefaultPictureAdjustment(HSIC& hsic);
    virtual status_t setPictureAdjustment(HSIC hsic);

    virtual ~LegacyMM();

  private:
    sp<DisplayMode> getDisplayModeById(int32_t id);
    int getNumDisplayModes();

    void* mLibHandle;

    int (*disp_api_init)(int32_t);
    int (*disp_api_get_color_balance_range)(int32_t, void*);
    int (*disp_api_set_color_balance)(int32_t, int);
    int (*disp_api_get_color_balance)(int32_t, int*);
    int (*disp_api_get_num_display_modes)(int32_t, int32_t, int*);
    int (*disp_api_get_display_modes)(int32_t, int32_t, void*, int);
    int (*disp_api_get_active_display_mode)(int32_t, int*, uint32_t*);
    int (*disp_api_set_active_display_mode)(int32_t, int);
    int (*disp_api_set_default_display_mode)(int32_t, int);
    int (*disp_api_get_default_display_mode)(int32_t, int*);
    int (*disp_api_get_pa_range)(int32_t, void*);
    int (*disp_api_get_pa_config)(int32_t, void*);
    int (*disp_api_set_pa_config)(int32_t, void*);
    int (*disp_api_supported)(int32_t, int32_t);
};
};

#endif
