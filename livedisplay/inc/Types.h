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

#ifndef CYNGN_LIVEDISPLAY_TYPES_H
#define CYNGN_LIVEDISPLAY_TYPES_H

#include <utils/RefBase.h>
#include <utils/String8.h>

namespace android {

class Range {
  public:
    Range() : max(0), min(0), step(0) {
    }
    Range(int32_t _min, int32_t _max) : max(_max), min(_min) {
    }
    ~Range() {
    }

    int32_t max;
    int32_t min;
    uint32_t step;

    bool isNonZero() {
        return min != 0 || max != 0;
    }
};

class FloatRange {
  public:
    FloatRange() : max(0.0), min(0.0), step(0.0) {
    }
    FloatRange(float _min, float _max) : max(_max), min(_min) {
    }
    ~FloatRange() {
    }

    float max;
    float min;
    float step;

    bool isNonZero() {
        return min != 0.0 || max != 0.0;
    }
};

class HSIC {
  public:
    HSIC() : hue(0), saturation(0), intensity(0), contrast(0), saturationThreshold(0) {
    }
    HSIC(int32_t _hue, float _saturation, float _intensity, float _contrast,
         float _saturationThreshold)
        : hue(_hue),
          saturation(_saturation),
          intensity(_intensity),
          contrast(_contrast),
          saturationThreshold(_saturationThreshold) {
    }
    ~HSIC() {
    }

    void setTo(HSIC o) {
        hue = o.hue;
        saturation = o.saturation;
        intensity = o.intensity;
        contrast = o.contrast;
        saturationThreshold = o.saturationThreshold;
    }

    int32_t hue;
    float saturation;
    float intensity;
    float contrast;
    float saturationThreshold;
};

class HSICRanges {
  public:
    HSICRanges() {
    }
    HSICRanges(Range _hue, FloatRange _saturation, FloatRange _intensity,
            FloatRange _contrast, FloatRange _saturationThreshold)
        : hue(_hue),
          saturation(_saturation),
          intensity(_intensity),
          contrast(_contrast),
          saturationThreshold(_saturationThreshold) {
    }

    Range hue;
    FloatRange saturation;
    FloatRange intensity;
    FloatRange contrast;
    FloatRange saturationThreshold;

    bool isValid() {
        return hue.isNonZero() && saturation.isNonZero() &&
               intensity.isNonZero() && contrast.isNonZero();
    }
};

class DisplayMode : public RefBase {
  public:
    DisplayMode() : id(-1) {
    }

    DisplayMode(int32_t _id, const char* _name, size_t _len) : id(_id) {
        name.setTo(_name, _len);
    }

    int32_t id;
    String8 name;

    uint32_t privFlags;
    String8 privData;
};

enum Level { OFF = -1, LOW, MEDIUM, HIGH, AUTO };

enum Feature {
    DISPLAY_MODES = 0x1,
    COLOR_TEMPERATURE = 0x2,
    OUTDOOR_MODE = 0x4,
    ADAPTIVE_BACKLIGHT = 0x8,
    PICTURE_ADJUSTMENT = 0x10,
    MAX = PICTURE_ADJUSTMENT
};
};

#endif
