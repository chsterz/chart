#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>

#include <cairomm/cairomm.h>

#include "types.h"

namespace pie
{

//Settings and Lengths
///////////////////////////////////////////////////////////////////////////////////////////////////

	//dimenstions of surface
	const float sheight = 512;
	const float swidth = 512;

	//margins
	const float margin_top = 15;
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

	//fonts
	const std::string fontFace="Source Sans Pro";
	Color fontColor = Color(128,128,128,255);
	const int fontSize = 14;
	//font extends
	Cairo::FontExtents fontExtents;
	Cairo::TextExtents textExtents;

///////////////////////////////////////////////////////////////////////////////////////////////////

	//readable Output Labels
	std::string shortLabel(float value)
	{
		if(value == 0.0f) return "0";
		//Kilo Mega Giga...
		const std::vector<std::string> suffix = {"", "K", "M", "G", "T", "E"};
		int index = std::log(value)/std::log(1000);
		value /= std::pow(1000, index);
		std::stringstream ss;
		ss << std::setprecision(std::log10(value)+1) << value << suffix[index];
		return ss.str();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////

	//reasonable axis lengths
	float computeSteps(float maxValue)
	{
			if (maxValue <= 0.0)
				return 0.0;

			const double lastPowerOf10 = std::pow(10.0, std::floor(std::round(std::log(maxValue) / std::log(10.0) * 1.0e6) / 1.0e6));

			for (const auto stepRelative : {2.5, 2.0, 1.0, 0.5, 0.25})
			{
				const double step = stepRelative * lastPowerOf10;

				if (std::ceil(maxValue / step) >= 3 && std::ceil(maxValue / step) < 6)
					return step;
			}

			return 0.0;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////

class BarChart
{
private:
	Cairo::RefPtr<Cairo::PdfSurface> surface;
	Cairo::RefPtr<Cairo::Context> cr;

public:
///////////////////////////////////////////////////////////////////////////////////////////////////

	BarChart(const DataSet &dataset, const std::string& fileName, std::vector<Color> colors = pie::defaultColors, const std::string &title="")
	{
		surface = Cairo::PdfSurface::create(fileName, swidth, sheight);
		cr = Cairo::Context::create(surface);

		//drawDebug(cr);

		drawContent(dataset, colors);
		drawXAxis();
		drawYAxis();
		drawTitle(title);

		surface->finish();
	}


///////////////////////////////////////////////////////////////////////////////////////////////////

	void drawContent(const DataSet & dataset, const std::vector<Color> colors)
	{
		size_t numberOfBars = dataset.size();
		float spacerSpace = (numberOfBars + 1)*spacer;
		float barWidth = (width - spacerSpace) / numberOfBars;

		float maxValue = *std::max_element(dataset.cbegin(), dataset.cend());
		float stepSize = computeSteps(maxValue);
		float ySteps = std::ceil(maxValue/stepSize);
		float maxYAxis = stepSize*ySteps;

		float visualStepSize = height/ySteps;

		for(int i=0; i <= ySteps; i++)
		{
			//draw y axis numbers
			cr->select_font_face(fontFace, Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_NORMAL);
			cr->set_source_rgba(fontColor.r(), fontColor.g(), fontColor.b(), fontColor.a());
			cr->set_font_size(fontSize);
			std::string label = shortLabel(stepSize*i);
			cr->get_text_extents(label, textExtents);
			cr->move_to(ox - textExtents.width, oy + -1.0f*i*visualStepSize + textExtents.height/2);
			cr->show_text(label);

			//draw lines
			cr->move_to(ox, oy + -1.0f*i*visualStepSize);
			cr->set_source_rgba(fontColor.r(), fontColor.g(), fontColor.b(), fontColor.a());
			cr->line_to(ox+width, oy + -1.0f*i*visualStepSize);
			cr->stroke();

		}

		//draw the bars
		for (size_t index = 0; index < numberOfBars; index++)
		{
			Color currentColor = colors[index%colors.size()];
			cr->set_line_width(1.0f);
			cr->set_source_rgba(currentColor.r(),currentColor.g(),currentColor.b(),currentColor.a());
			float value = dataset[index];
			float barHeight = (value / maxYAxis) * height;
			cr->rectangle(ox + barWidth * index + spacer*(index+1), oy, barWidth, -1*barHeight);
			cr->fill();
		}
	}


///////////////////////////////////////////////////////////////////////////////////////////////////

	void drawXAxis()
	{
		cr->set_line_width(1.0f);
		cr->set_source_rgba(0.0f,0.0f,0.0f,1.0f);
		cr->move_to(ox, oy);
		cr->line_to(ox + width, oy);
		cr->stroke();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////

	void drawYAxis()
	{
		cr->set_line_width(1.0f);
		cr->set_source_rgba(fontColor.r(), fontColor.g(), fontColor.b(), fontColor.a());
		cr->move_to(ox, oy);
		cr->line_to(ox, oy + -1.0f*height);
		cr->stroke();
	}


///////////////////////////////////////////////////////////////////////////////////////////////////

	void drawTitle(const std::string &title)
	{
		cr->select_font_face(fontFace, Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_NORMAL);
		cr->set_source_rgba(0.0f,0.0f,0.0f,1.0f);
		cr->set_font_size(fontSize+8);
		cr->get_text_extents(title, textExtents);
		cr->move_to(ox + width/2 - textExtents.width/2, oy -1.0f * -(textExtents.height+5));
		cr->show_text(title);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////

	void drawDebug()
	{
		cr->set_line_width(1.0f);
		cr->set_source_rgba(pie::debugColor.r(),pie::debugColor.g(),pie::debugColor.b(),pie::debugColor.a());

		//chart area debug
		cr->rectangle(ox, oy, width, -1*height);
		cr->stroke();

		//x-axis debug
		cr->rectangle(x_axis_ox, x_axis_oy, x_axis_width, -1* x_axis_height);
		cr->stroke();

		//y-axis debug
		cr->rectangle(y_axis_ox, y_axis_oy, y_axis_width, -1* y_axis_height);
		cr->stroke();
	}

};

}
