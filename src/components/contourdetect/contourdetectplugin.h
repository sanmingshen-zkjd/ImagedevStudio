#pragma once

#include "plugin_type.h"
#include "contourdetectmodel.h"

using BrightnessContrastPlugin = PluginBase<typestring_is("PreProcess Layer"), 0xf7aa1b, 1>;

struct ImageContourDetectPlugin {
    using Model = ContourDetectModel;
    static constexpr const char* name = "ContourDetect";
    using PluginType = BrightnessContrastPlugin;
};
