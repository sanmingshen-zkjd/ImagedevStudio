#pragma once

#include "plugin_type.h"
#include "brightnesscontrastmodel.h"

using BrightnessContrastPlugin = PluginBase<typestring_is("PreProcess Layer"), 0xf7aa1b, 1>;

struct ImageBrightnessContrastPlugin {
    using Model = BrightnessContrastModel;
    static constexpr const char* name = "BrightnessContrast";
    using PluginType = BrightnessContrastPlugin;
};