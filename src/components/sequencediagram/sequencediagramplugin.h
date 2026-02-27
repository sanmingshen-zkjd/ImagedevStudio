#pragma once

#include "plugin_type.h"
#include "sequencediagrammodel.h"

using BrightnessContrastPlugin = PluginBase<typestring_is("PreProcess Layer"), 0xf7aa1b, 1>;

struct ImageSequenceDiagramPlugin {
    using Model = SequenceDiagramModel;
    static constexpr const char* name = "SequenceDiagram";
    using PluginType = BrightnessContrastPlugin;
};
