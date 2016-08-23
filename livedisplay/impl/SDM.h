#ifndef CYNGN_LIVEDISPLAYSDM_H
#define CYNGN_LIVEDISPLAYSDM_H

#include <LiveDisplayBackend.h>

#define SDM_DISP_LIB "libsdm-disp-apis.so"

#define DPPS_BUF_SIZE 64

#define FOSS_SUPPORTED "foss:support"
#define FOSS_ON "foss:on"
#define FOSS_OFF "foss:off"
#define FOSS_STATUS "foss:status"

#define SRGB_NODE "/sys/class/graphics/fb0/srgb"
#define SRGB_NODE_ID 601

#define PRIV_MODE_FLAG_SDM 1
#define PRIV_MODE_FLAG_SYSFS 2

namespace android {

struct hsic_data {
    int32_t hue;
    float saturation;
    float intensity;
    float contrast;
    float saturationThreshold;
};

struct hsic_config {
    uint32_t unused;
    hsic_data data;
};

struct hsic_int_range {
    int32_t max;
    int32_t min;
    uint32_t step;
};

struct hsic_float_range {
    float max;
    float min;
    float step;
};

struct hsic_ranges {
    uint32_t unused;
    struct hsic_int_range hue;
    struct hsic_float_range saturation;
    struct hsic_float_range intensity;
    struct hsic_float_range contrast;
    struct hsic_float_range saturationThreshold;
};

class SDM : public LiveDisplayBackend {
  public:
    virtual status_t initialize();
    virtual status_t deinitialize();
    virtual bool hasFeature(Feature feature);

    virtual status_t setAdaptiveBacklightEnabled(bool enabled);
    virtual bool isAdaptiveBacklightEnabled();

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

    virtual ~SDM();

  private:
    status_t loadVendorLibrary();

    sp<DisplayMode> getLocalSRGBMode();
    sp<DisplayMode> getDisplayModeById(int32_t id);
    status_t setModeState(sp<DisplayMode> mode, bool state);
    uint32_t getNumDisplayModes();

    int64_t mHandle;
    bool mCachedFOSSStatus;
    int32_t mActiveModeId;

    HSIC mDefaultPictureAdjustment;

    void* mLibHandle;

    int32_t (*disp_api_init)(int64_t*, uint32_t);
    int32_t (*disp_api_deinit)(int64_t, uint32_t);
    int32_t (*disp_api_get_global_color_balance_range)(int64_t, uint32_t, void*);
    int32_t (*disp_api_set_global_color_balance)(int64_t, uint32_t, int32_t, uint32_t);
    int32_t (*disp_api_get_global_color_balance)(int64_t, uint32_t, int32_t*, uint32_t*);
    int32_t (*disp_api_get_num_display_modes)(int64_t, uint32_t, int32_t, int32_t*, uint32_t*);
    int32_t (*disp_api_get_display_modes)(int64_t, uint32_t, int32_t, void*, int32_t, uint32_t*);
    int32_t (*disp_api_get_active_display_mode)(int64_t, uint32_t, int32_t*, uint32_t*, uint32_t*);
    int32_t (*disp_api_set_active_display_mode)(int64_t, uint32_t, int32_t, uint32_t);
    int32_t (*disp_api_set_default_display_mode)(int64_t, uint32_t, int32_t, uint32_t);
    int32_t (*disp_api_get_default_display_mode)(int64_t, uint32_t, int32_t*, uint32_t*);
    int32_t (*disp_api_get_global_pa_range)(int64_t, uint32_t, void*);
    int32_t (*disp_api_get_global_pa_config)(int64_t, uint32_t, uint32_t*, void*);
    int32_t (*disp_api_set_global_pa_config)(int64_t, uint32_t, uint32_t, void*);
    int32_t (*disp_api_get_feature_version)(int64_t, uint32_t, void*, uint32_t*);
};
};

#endif
