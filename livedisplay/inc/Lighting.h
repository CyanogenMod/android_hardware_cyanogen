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

#ifndef CYNGN_DISPLAYFEATURE_H
#define CYNGN_DISPLAYFEATURE_H

#include <utils/Errors.h>

namespace android {

// Natural light sources
const int CANDLE[] = {255, 147, 41};
const int TUNGSTEN_40W[] = {255, 197, 143};
const int TUNGSTEN_100W[] = {255, 214, 170};
const int HALOGEN[] = {255, 241, 224};
const int CARBON_ARC[] = {255, 250, 244};
const int HIGH_NOON_SUN[] = {255, 255, 251};
const int DIRECT_SUNLIGHT[] = {255, 255, 255};
const int OVERCAST_SKY[] = {201, 226, 255};
const int CLEAR_BLUE_SKY[] = {64, 156, 255};

// Fluorescent lights
const int WARM[] = {255, 244, 229};
const int STANDARD[] = {244, 255, 250};
const int COOL_WHITE[] = {212, 235, 255};
const int FULL_SPECTRUM[] = {255, 244, 242};
const int GROW_LIGHT[] = {255, 239, 247};
const int BLACK_LIGHT[] = {167, 0, 255};

// Gas lights
const int MERCURY_VAPOR[] = {216, 247, 255};
const int SODIUM_VAPOR[] = {255, 209, 178};
const int METAL_HALIDE[] = {242, 252, 255};
const int HP_SODIUM[] = {255, 183, 76};
};

#endif
