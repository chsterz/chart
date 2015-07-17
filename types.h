#pragma once

class Color: public std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>
{
public:
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>(r,g,b,a)
	{}

	template <class T = float>
	T r();
	template <class T = float>
	T g();
	template <class T = float>
	T b();
	template <class T = float>
	T a();
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <>
float Color::r<float>()
{return std::get<0>(*this)/255.0f;}

template <>
unsigned char Color::r<unsigned char>()
{return std::get<0>(*this);}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <>
float Color::g<float>()
{return std::get<1>(*this)/255.0f;}

template <>
unsigned char Color::g<unsigned char>()
{return std::get<1>(*this);}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <>
float Color::b<float>()
{return std::get<2>(*this)/255.0f;}

template <>
unsigned char Color::b<unsigned char>()
{return std::get<2>(*this);}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <>
float Color::a<float>()
{return std::get<3>(*this)/255.0f;}

template <>
unsigned char Color::a<unsigned char>()
{return std::get<3>(*this);}

///////////////////////////////////////////////////////////////////////////////////////////////////


using DataSet = std::vector<float>;
using Data = std::vector<DataSet>;

///////////////////////////////////////////////////////////////////////////////////////////////////

bool rotate(Data &data)
{
	size_t dataLength = data.size();
	size_t dataSetLength = data[0].size();
	//first check if each dataset has the same lengths
	for (size_t i = 0; i < dataLength; i++)
	{if (data[i].size() != dataSetLength) return false;}

	Data result;
	for(size_t j = 0; j < dataSetLength; j++)
	{
		DataSet dataSet;
		for(size_t i = 0; i < dataLength; i++)
		{
			dataSet.push_back(data[i][j]);
		}
		result.push_back(dataSet);
	}
	data = result;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////


using LabelSet = std::vector<std::string>;
using Labels = std::vector<LabelSet>;

namespace pie
{
	std::vector<Color> defaultColors = { Color(255,0,0,255), Color(0,255,0,255), Color(0,0,255,255)};
	Color debugColor = Color(255,0,0,50);
}
