#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "types.h"
#include "terminalcolors.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
void resetStream(std::istream &file)
{
	file.clear();
	file.seekg(std::ios::beg);
}


///////////////////////////////////////////////////////////////////////////////////////////////////

bool isSeparator(const char c)
{
	return (c == ',' or c == '\t' or c == ' ');
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool isNewline(const char c)
{
	return (c == '\n');
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool isCSV(std::istream &file)
{
	std::stringstream fileContents;

	fileContents << file.rdbuf();

	//check first data for label
	//assuming labels begin with a letter
	bool message = false;
	while (not std::isdigit(fileContents.peek()))
	{
		if (not message)
		{
			message = true;
			std::cout << text::blue("[Info]") << " Assuming first Row as Labels" << std::endl;
		}

		std::string dummyLabel;
		fileContents >> dummyLabel;

		while (isSeparator(fileContents.peek()) or isNewline(fileContents.peek()))
			fileContents.ignore();
	}

	//Extract one dummy value
	float dummy;
	fileContents >> dummy;

	//Is there a delimiter after the first value?
	if (isSeparator(fileContents.peek()))
	{
		resetStream(file);
		return true;
	}

	resetStream(file);
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Data parseCSV(std::istream &file)
{
	std::string line;
	Data data;

	//See if there are labels to read
	if (std::isalpha(file.peek()))
	{
		std::getline(file, line);
		std::stringstream linestream(line);
		std::string label;

		while (linestream >> label)
			data.columnLabels.push_back(label);
	}

	//Read the actual data
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		std::stringstream linestream(line);
		DataSet linedata;
		float currentValue;
		while (linestream >> currentValue)
		{
			linedata.push_back(currentValue);
			while (isSeparator(linestream.peek()))
				linestream.ignore();
		}
		data.push_back(linedata);
	}

	return data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool isRowData(std::istream &file)
{
	std::stringstream fileContents;

	fileContents << file.rdbuf();
	//Extract one dummy value
	float dummy;
	fileContents >> dummy;

	//Is there a newline after the first value?
	if (fileContents.peek() == '\n')
	{
		resetStream(file);
		return true;
	}

	resetStream(file);
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Data parseRowData(std::istream &file)
{
	std::string line;

	Data data;
	DataSet rowsData;
	while (std::getline(file, line))
	{
		std::stringstream linestream(line);

		float currentValue;
		if (linestream.str().empty())
		{
			data.push_back(rowsData);
			rowsData.clear();
			continue;
		}
		linestream >> currentValue;
		rowsData.push_back(currentValue);
	}
	data.push_back(rowsData);
	return data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool isColorFile(std::istream &file)
{
	std::stringstream fileContents;

	fileContents << file.rdbuf();
	//Are all colors in Hex :)
	if (fileContents.peek() != '#')
		return false;
	fileContents.ignore();
	std::string dummy;
	fileContents >> dummy;

	//std::hex expects a 0x in front
	dummy = "0x" + dummy;
	unsigned int color;
	std::stringstream ss(dummy);
	ss >> std::hex >> color;

	//Is Each Color on a new line
	if (fileContents.peek() != '\n')
	{
		resetStream(file);
		return false;
	}

	resetStream(file);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<Color> parseColors(std::istream &file)
{
	std::vector<Color> result;
	std::string line;

	while (std::getline(file, line))
	{
		//take away #, replace with 0x
		line = "0x"+line.erase(0,1);
		unsigned int color;
		std::stringstream ss(line);
		ss >> std::hex >> color;

		unsigned char r = (color>>24) & 0xff;
		unsigned char g = (color>>16) & 0xff;
		unsigned char b = (color>>8)  & 0xff;
		unsigned char a = color       & 0xff;

		result.push_back(Color(r,g,b,a));
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
