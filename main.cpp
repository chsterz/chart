#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "terminalcolors.h"
#include "fileparsing.h"
#include "drawing.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
	//Help Message
	po::options_description desc("All Options");

	//All program options
	desc.add_options()
			("help,h","Show this Message")
			("colors,c",po::value<int>(), "colors for this chart" )
			("data-file,f", po::value<std::string>(), "A file plot");

	//Parse command line options
	po::variables_map parameters;
	po::store(po::parse_command_line(argc, argv, desc), parameters);
	po::notify(parameters);

	//Help or error
	if(parameters.count("help") or parameters.empty())
	{
		std::cout << desc << std::endl;
		return EXIT_FAILURE;
	}


	//Get Data from File
	Data data;
	if( parameters.count("data-file") )
	{
		//File exists?
		std::string fileName = parameters["data-file"].as<std::string>();
		if( not fs::exists(fileName) )
		{
			std::cerr << color::red("[Error]")
					  << " No such file: '" + fileName + "'"
					  << std::endl;
			return EXIT_FAILURE;
		}
		//Read Data
		if( isCSV(fileName) )
			data = parseCSV(fileName);
		else if( isRowData(fileName) )
			data = parseRowData(fileName);
		else
		{
			std::cerr << color::red("[Error]")
					  << " Cannot understand file format of '" + fileName + "'"
					  << std::endl;

			return EXIT_FAILURE;
		}
	}

	//Get Colors
	std::vector<Color> colors;
	if( parameters.count("colors") )
	{
		//Color File exists?
		std::string fileName = parameters["colors"].as<std::string>();
		if( not fs::exists(fileName) )
		{
			std::cerr << color::red("[Error]")
					  << " No such color file: '" + fileName + "'"
					  << std::endl;
			return EXIT_FAILURE;
		}
		if( isColorFile(fileName))
			colors = parseColors(fileName);
		else
		{
			std::cerr << color::red("[Error]")
					  << " Cannot understand color file format of '" + fileName + "'"
					  << std::endl;
		}
	}
	//Use the default if no file is provided
	else
		colors = pie::defaultColors;


	return EXIT_SUCCESS;
}
