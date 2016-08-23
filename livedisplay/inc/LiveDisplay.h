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

#ifndef CYNGN_LIVEDISPLAYBASE_H
#define CYNGN_LIVEDISPLAYBASE_H

#include <utils/Log.h>
#include <utils/Mutex.h>
#include <utils/Singleton.h>

#include "LiveDisplayBackend.h"
#include "Types.h"

namespace android {

class LiveDisplay : public LiveDisplayAPI, public Singleton<LiveDisplay> {
    friend class Singleton;

  public:
    bool hasFeature(Feature f) {
        return connect() && (mFeatures & (uint32_t)f);
    }

    uint32_t getSupportedFeatures();

    void reset();

    virtual status_t setAdaptiveBacklightEnabled(bool enabled);
    virtual bool isAdaptiveBacklightEnabled();

    virtual status_t setOutdoorModeEnabled(bool enabled);
    virtual bool isOutdoorModeEnabled();

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

    virtual ~LiveDisplay();
    LiveDisplay();

  private:
    uint32_t mFeatures;
    bool mConnected;

    bool check(Feature f);
    bool connect();
    void error(const char* msg = NULL, ...);
    bool isConnected() {
        return mConnected;
    }

    void addFeature(Feature f) {
        mFeatures |= (uint32_t)f;
    };
    void clearFeatures() {
        mFeatures = 0;
    };

    LiveDisplayBackend* mBackend;
    Mutex mLock;
};
};

#endif
