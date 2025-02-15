#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace Assets
{
    struct TextAsset
    {
        const std::string name;
        const std::string text;
    };

    extern std::unordered_map<std::string, const TextAsset> textAssets;
}
