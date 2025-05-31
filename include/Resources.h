#pragma once

#include "Assets.h"
#include <string>

namespace Resources
{
	const Assets::TextAsset* LoadTextAsset(const std::string &);
	const Assets::ImageAsset* LoadImageAsset(const std::string &);
}