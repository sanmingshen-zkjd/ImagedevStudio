#pragma once

#include "plugin_type.h"
#include "imagesourcemodel.h"

using SourcePlugin = PluginBase<typestring_is("image Source Layer"), 0x90bb3e, 1>;

struct ImageSourcePlugin {
    using Model = ImageSourceModel;
    static constexpr const char* name = "ImageSource";
    using PluginType = SourcePlugin;
};
