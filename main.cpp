#include <cstdlib>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <boost/program_options.hpp>
#include <stdio.h>

#include "terminalcolors.h"
#include "fileparsing.h"
#include "drawing.h"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
	//Help Message
	po::options_description desc( text::bold("Example 1: ") + "$data-producer | pie\n"
								 +text::bold("Example 2:")+" $pie -f datadfile [-c colorfile]\n\n"
								 +text::bold("All Options:"));

	//All program options
	desc.add_options()
			("help,h","Show this Message")
			("colors,c",po::value<std::string>(), "colors for this chart" )
			("data-file,f", po::value<std::string>(), "A file plot");

	//Parse command line options
	po::variables_map parameters;
	po::store(po::parse_command_line(argc, argv, desc), parameters);
	po::notify(parameters);

	//Help or error
	if (parameters.count("help"))
	{
		std::cout << desc << std::endl;
		return EXIT_FAILURE;
	}


	//Get Data from File
	Data data;
	if (parameters.count("data-file"))
	{
		//File exists?
		std::string fileName = parameters["data-file"].as<std::string>();
		std::ifstream file(fileName);

		if (not file.is_open())
		{
			std::cerr << text::red("[Error]") << " No such file: '" + fileName + "'" << std::endl;
			return EXIT_FAILURE;
		}

		if (isCSV(file))
			data = parseCSV(file);
		else if (isRowData(file))
			data = parseRowData(file);
		else
		{
			std::cerr << text::red("[Error]") << " Cannot understand file format of '" + fileName + "'" << std::endl;
			return EXIT_FAILURE;
		}
	}

	//First check if pipe is filled, if used
	bool filledPipe = not isatty(0);
	if (data.empty() and not filledPipe)
	{
		std::cerr << text::red("Error") << " Could not find data." << std::endl << "Please provide data via file or pipe!" << std::endl;
		std::cout << desc << std::endl;
		return EXIT_FAILURE;
	}

	//Dont allow data from file and pipe
	if (not data.empty() and filledPipe)
	{
		std::cerr << text::red("Error") << " Double data input (file & pipe)." << std::endl << "Please specify data from exactly one source: file or pipe!" << std::endl;
		return EXIT_FAILURE;
	}

	if(filledPipe)
	{
		std::stringstream ss;
		ss << std::cin.rdbuf();
		if (isCSV(ss))
			data = parseCSV(ss);
		else if (isRowData(ss))
			data = parseRowData(ss);
		else
		{
			std::cerr << text::red("[Error]") << " Cannot understand file format of data in pipe." << std::endl;
			return EXIT_FAILURE;
		}
	}

	//Get Colors
	std::vector<Color> colors;
	if (parameters.count("colors"))
	{
		//Color File exists?
		std::string fileName = parameters["colors"].as<std::string>();
		std::ifstream file(fileName);
		if (not file.is_open())
		{
			std::cerr << text::red("[Error]") << " No such color file: '" + fileName + "'" << std::endl;
			return EXIT_FAILURE;
		}
		if (isColorFile(file))
			colors = parseColors(file);
		else
		{
			std::cerr << text::red("[Error]") << " Cannot understand color file format of '" + fileName + "'" << std::endl << "Just add hex-colors like #ff00ffff in separate lines." << std::endl;
			return EXIT_FAILURE;
		}
	}
	//Use the default if no file is provided
	else colors = pie::defaultColors;


	//latcheck if data empty
	if (data.empty())
	{
		std::cerr << text::red("[Error]") << " Data seems to be empty." << std::endl << "Check if your input is filled." << std::endl;
		return EXIT_FAILURE;
	}


	//#1 draw barcharts from the data
	for(size_t i = 0; i < data.size(); i++)
	{
		if(data[i].empty()) continue;
		std::stringstream fileNameStream;
		fileNameStream << "Chart_" << i << ".pdf";
		pie::BarChart(data[i], fileNameStream.str(), colors);
	}

	return EXIT_SUCCESS;
}
