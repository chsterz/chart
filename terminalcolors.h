#pragma once
#include <string>

namespace color
{
	const std::string redModifier = "\x1b[31m";
	const std::string clearColor = "\x1b[39;49m";

	const std::string clearAll = "\x1b[0m";


	std::string red(const std::string &str)
	{return redModifier + str + clearColor;}
}


