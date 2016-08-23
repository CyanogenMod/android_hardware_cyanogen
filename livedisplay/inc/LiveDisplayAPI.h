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

#ifndef CYNGN_LIVEDISPLAYAPI_H
#define CYNGN_LIVEDISPLAYAPI_H

#include <utils/Errors.h>
#include <utils/List.h>

#include <Types.h>

namespace android {

class LiveDisplayAPI {
  public:
    virtual status_t setAdaptiveBacklightEnabled(bool enabled) = 0;
    virtual bool isAdaptiveBacklightEnabled() = 0;

    virtual status_t setOutdoorModeEnabled(bool enabled) = 0;
    virtual bool isOutdoorModeEnabled() = 0;

    virtual status_t getColorBalanceRange(Range& range) = 0;
    virtual status_t setColorBalance(int32_t balance) = 0;
    virtual int32_t getColorBalance() = 0;

    virtual status_t getDisplayModes(List<sp<DisplayMode>>& profiles) = 0;
    virtual status_t setDisplayMode(int32_t modeID, bool makeDefault) = 0;
    virtual sp<DisplayMode> getCurrentDisplayMode() = 0;
    virtual sp<DisplayMode> getDefaultDisplayMode() = 0;

    virtual status_t getPictureAdjustmentRanges(HSICRanges& ranges) = 0;
    virtual status_t getPictureAdjustment(HSIC& hsic) = 0;
    virtual status_t getDefaultPictureAdjustment(HSIC& hsic) = 0;
    virtual status_t setPictureAdjustment(HSIC hsic) = 0;

    virtual ~LiveDisplayAPI() {
    }
};
};

#endif
