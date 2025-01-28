#include "Resources.h"

#include "Debug.h"
#include <format>

const Assets::TextAsset* Resources::LoadTextAsset(const std::string &filePath)
{
#ifdef DEBUG
    ASSERT(Assets::textAssets.contains(filePath), true, std::format("Missing resource, {}", filePath));
#endif // DEBUG

    return &Assets::textAssets[filePath];
}