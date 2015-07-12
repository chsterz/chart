#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <algorithm>

#include <cairomm/cairomm.h>

#include "types.h"

namespace pie
{
	//dimenstions of surface
	const float sheight = 512;
	const float swidth = 512;

	//margins
	const float margin_top = 5;
	const float margin_bottom = 5;
	const float margin_left = 5;
	const float margin_right = 5;

	//axes
	const float x_axis_height = 30;
	const float y_axis_width = 30;
	const float x_axis_width = swidth - margin_left - margin_right - y_axis_width;
	const float y_axis_height = sheight - margin_bottom - margin_top - x_axis_height;

	const float x_axis_ox = margin_left + y_axis_width;
	const float x_axis_oy = sheight - margin_bottom;

	const float y_axis_ox = margin_left;
	const float y_axis_oy = sheight - margin_bottom - x_axis_height;

	//dimensions of drawable area
	const float height = sheight - margin_top - margin_bottom - x_axis_height;
	const float width = swidth - margin_left - margin_right - y_axis_width;

	//origin of actual drawing area
	const float ox = margin_left + y_axis_width;
	const float oy = sheight - margin_bottom - x_axis_height;

	//spacers
	const float spacer = 15;

	//debug mode
	const bool debug = false;

class BarChart
{
public:
	BarChart(const DataSet &dataset, const std::string& fileName, std::vector<Color> colors = pie::defaultColors)
	{
		Cairo::RefPtr<Cairo::PdfSurface> surface = Cairo::PdfSurface::create(fileName, swidth, sheight);
		Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

		//draw background
		cr->set_line_width(1.0f);
		cr->set_source_rgb(1.0f, 1.0f, 1.0f);

		size_t numberOfBars = dataset.size();
		float spacerSpace = (numberOfBars + 1)*spacer;
		float barWidth = (width - spacerSpace) / numberOfBars;

		cr->set_line_width(1.0f);
		//chart area debug
		if (debug)
		{
			cr->set_source_rgba(pie::debugColor.r(),pie::debugColor.g(),pie::debugColor.b(),pie::debugColor.a());
			cr->rectangle(ox, oy, width, -1*height);
			cr->stroke();
		}

		//x-axis debug
		if (debug)
		{
			cr->set_source_rgba(pie::debugColor.r(),pie::debugColor.g(),pie::debugColor.b(),pie::debugColor.a());
			cr->rectangle(x_axis_ox, x_axis_oy, x_axis_width, -1* x_axis_height);
			cr->stroke();
		}

		//y-axis debug
		if (debug)
		{
			cr->set_source_rgba(pie::debugColor.r(),pie::debugColor.g(),pie::debugColor.b(),pie::debugColor.a());
			cr->rectangle(y_axis_ox, y_axis_oy, y_axis_width, -1* y_axis_height);
			cr->stroke();
		}

		float maxValue = *std::max_element(dataset.cbegin(), dataset.cend());


		for (size_t index = 0; index < numberOfBars; index++)
		{
			Color currentColor = colors[index%colors.size()];
			cr->set_line_width(1.0f);
			cr->set_source_rgba(currentColor.r(),currentColor.g(),currentColor.b(),currentColor.a());
			float value = dataset[index];
			float barHeight = (value / maxValue) * height;
			cr->rectangle(ox + barWidth * index + spacer*(index+1), oy, barWidth, -1*barHeight);
			cr->fill();
		}

		//draw x-axis
		cr->set_line_width(1.0f);
		cr->set_source_rgba(0.0f,0.0f,0.0f,1.0f);
		cr->move_to(ox, oy);
		cr->line_to(ox + width, oy);
		cr->stroke();

		surface->finish();
	}
};


}
