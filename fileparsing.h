#pragma once

#include <fstream>
#include <sstream>

#include "drawing.h"

using Dataset = std::vector<float>;
using Data = std::vector<Dataset>;

///////////////////////////////////////////////////////////////////////////////////////////////////

bool isCSV(const std::string &fileName)
{
	std::ifstream file(fileName);
	std::stringstream fileContents;

	fileContents << file.rdbuf();
	//Extract one dummy value
	float dummy;
	fileContents >> dummy;

	//Is there a delimiter after the first value?
	if( fileContents.peek() == ',' || fileContents.peek() == ' ' || fileContents.peek() == '\t')
		return true;

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Data parseCSV(const std::string &fileName)
{
	std::ifstream file(fileName);
	std::string line;

	Data data;
	while(std::getline(file, line))
	{
		std::stringstream linestream(line);
		Dataset linedata;
		float currentValue;
		while(linestream >> currentValue)
		{
			linedata.push_back(currentValue);
			if( linestream.peek() == ',' || linestream.peek() == ' ' || linestream.peek() == '\t')
				linestream.ignore();
		}
		data.push_back(linedata);
	}

	return data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool isRowData(const std::string &fileName)
{
	std::ifstream file(fileName);
	std::stringstream fileContents;

	fileContents << file.rdbuf();
	//Extract one dummy value
	float dummy;
	fileContents >> dummy;

	//Is there a newline after the first value?
	while(file)
	if( fileContents.peek() == '\n' )
		return true;

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Data parseRowData(const std::string &fileName)
{
	std::ifstream file(fileName);
	std::string line;

	Data data;
	Dataset rowsData;
	while(std::getline(file, line))
	{
		std::stringstream linestream(line);

		float currentValue;
		if(linestream.str().empty())
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

bool isColorFile(const std::string &fileName)
{
	//NYI
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<Color> parseColors(const std::string &fileName)
{
	//NYI
	return pie::defaultColors;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
