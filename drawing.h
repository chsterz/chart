#pragma once

#include <vector>
#include <tuple>

using Color = std::tuple< float, float, float>;
namespace pie
{
	std::vector<Color> defaultColors = { Color(1.0,0.0,0.0), Color(0.0,0.0,1.0), Color(0.0,0.1,0.0)};
}
