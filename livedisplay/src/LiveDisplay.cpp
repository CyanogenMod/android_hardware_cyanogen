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

#include <cutils/properties.h>
#include <stdarg.h>

#include "LiveDisplay.h"

#include "LegacyMM.h"
#include "SDM.h"

namespace android {

ANDROID_SINGLETON_STATIC_INSTANCE(LiveDisplay)

LiveDisplay::LiveDisplay() : mConnected(false), mBackend(NULL) {
    char board[PROPERTY_VALUE_MAX];
    property_get("ro.board.platform", board, NULL);

    if (!strcmp(board, "msm8916") || !strcmp(board, "msm8939") || !strcmp(board, "msm8992") ||
        !strcmp(board, "msm8974") || !strcmp(board, "msm8994")) {
        mBackend = new LegacyMM();
    } else if (!strcmp(board, "msm8996") || !strcmp(board, "msm8937") ||
               !strcmp(board, "msm8953") || !strcmp(board, "msm8976")) {
        mBackend = new SDM();
    } else {
        mBackend = NULL;
        return;
    }
    ALOGD("Loaded LiveDisplay native interface");
}

LiveDisplay::~LiveDisplay() {
    reset();
}

void LiveDisplay::reset() {
    if (mConnected) {
        mBackend->deinitialize();
    }
    mFeatures = 0;
    mConnected = false;
}

void LiveDisplay::error(const char* msg, ...) {
    if (msg != NULL) {
        va_list args;
        ALOGE(msg, args);
    }

    reset();
}

bool LiveDisplay::connect() {
    if (mConnected) {
        return true;
    }

    mFeatures = 0;

    if (mBackend == NULL) {
        return false;
    }

    if (mBackend->initialize() != OK) {
        ALOGE("Failed to initialize backend!");
        return false;
    }

    for (uint32_t i = 1; i <= (uint32_t)Feature::MAX; i <<= 1) {
        Feature f = static_cast<Feature>(i);
        if (mBackend->hasFeature(f)) {
            addFeature(f);
        }
    }
    mConnected = true;

    return mFeatures > 0;
}

uint32_t LiveDisplay::getSupportedFeatures() {
    connect();
    return mFeatures;
}

bool LiveDisplay::check(Feature f) {
    return hasFeature(f) && connect();
}

//----------------------------------------------------------------------------/

status_t LiveDisplay::getDisplayModes(List<sp<DisplayMode>>& modes) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        rc = mBackend->getDisplayModes(modes);
        if (rc != OK) {
            error("Unable to fetch display modes!");
        }
    }
    return rc;
}

sp<DisplayMode> LiveDisplay::getDefaultDisplayMode() {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        return mBackend->getDefaultDisplayMode();
    }
    return nullptr;
}

sp<DisplayMode> LiveDisplay::getCurrentDisplayMode() {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        return mBackend->getCurrentDisplayMode();
    }
    return nullptr;
}

status_t LiveDisplay::setDisplayMode(int32_t modeID, bool makeDefault) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::DISPLAY_MODES)) {
        rc = mBackend->setDisplayMode(modeID, makeDefault);
        if (rc != OK) {
            error("Unable to set display mode!");
        }
    }
    return rc;
}

status_t LiveDisplay::getColorBalanceRange(Range& range) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_TEMPERATURE)) {
        rc = mBackend->getColorBalanceRange(range);
        if (rc != OK) {
            error("Unable to fetch color balance range!");
        }
    }
    return rc;
}

int LiveDisplay::getColorBalance() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_TEMPERATURE)) {
        return mBackend->getColorBalance();
    }

    return 0;
}

status_t LiveDisplay::setColorBalance(int value) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::COLOR_TEMPERATURE)) {
        rc = mBackend->setColorBalance(value);
        if (rc != OK) {
            error("Unable to set color balance!");
        }
    }
    return rc;
}

bool LiveDisplay::isOutdoorModeEnabled() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::OUTDOOR_MODE)) {
        return mBackend->isOutdoorModeEnabled();
    }
    return false;
}

status_t LiveDisplay::setOutdoorModeEnabled(bool enabled) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::OUTDOOR_MODE)) {
        rc = mBackend->setOutdoorModeEnabled(enabled);
        if (rc != OK) {
            error("Unable to toggle outdoor mode!");
        }
    }
    return rc;
}

bool LiveDisplay::isAdaptiveBacklightEnabled() {
    Mutex::Autolock _l(mLock);

    if (check(Feature::ADAPTIVE_BACKLIGHT)) {
        return mBackend->isAdaptiveBacklightEnabled();
    }
    return false;
}

status_t LiveDisplay::setAdaptiveBacklightEnabled(bool enabled) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::ADAPTIVE_BACKLIGHT)) {
        rc = mBackend->setAdaptiveBacklightEnabled(enabled);
        if (rc != OK) {
            error("Unable to set adaptive backlight state!");
        }
    }
    return rc;
}

status_t LiveDisplay::getPictureAdjustment(HSIC& hsic) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        rc = mBackend->setPictureAdjustment(hsic);
        if (rc != OK) {
            error("Unable to get picture adjustment!");
        }
    }
    return rc;
}

status_t LiveDisplay::getDefaultPictureAdjustment(HSIC& hsic) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        rc = mBackend->getDefaultPictureAdjustment(hsic);
        if (rc != OK) {
            error("Unable to get default picture adjustment!");
        }
    }
    return rc;
}


status_t LiveDisplay::setPictureAdjustment(HSIC hsic) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        rc = mBackend->setPictureAdjustment(hsic);
        if (rc != OK) {
            error("Unable to set picture adjustment!");
        }
    }
    return rc;
}

status_t LiveDisplay::getPictureAdjustmentRanges(HSICRanges& ranges) {
    status_t rc = NO_INIT;
    Mutex::Autolock _l(mLock);

    if (check(Feature::PICTURE_ADJUSTMENT)) {
        rc = mBackend->getPictureAdjustmentRanges(ranges);
        if (rc != OK) {
            error("Unable to get picture adjustment ranges!");
        }
    }
    return rc;
}
};
