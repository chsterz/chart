#pragma once
#include <string>

namespace text
{
	const std::string redModifier = "\x1b[31m";
	const std::string blueModifier = "\x1b[34m";

	//styles
	const std::string boldModifier = "\x1b[39;1m";

	//clears
	const std::string clearColor = "\x1b[39;49m";
	const std::string clearAll = "\x1b[0m";

	//color functions
	std::string red(const std::string &str)
	{return redModifier + str + clearColor;}

	std::string blue(const std::string &str)
	{return blueModifier + str + clearColor;}

	//style functions
	std::string bold(const std::string &str)
	{return boldModifier + str + clearAll;}
}


