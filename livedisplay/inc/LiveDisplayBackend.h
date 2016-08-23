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

#ifndef CYNGN_LIVEDISPLAYBACKEND_H
#define CYNGN_LIVEDISPLAYBACKEND_H

#include <utils/Errors.h>

#include <LiveDisplayAPI.h>

namespace android {

class LiveDisplayBackend : public LiveDisplayAPI {
  public:
    virtual status_t initialize() = 0;
    virtual status_t deinitialize() = 0;
    virtual bool hasFeature(Feature feature) = 0;

    virtual ~LiveDisplayBackend() {
    }
};
};

#endif
