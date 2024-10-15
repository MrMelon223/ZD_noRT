#ifndef ZDTEXTURE_H
#define ZDTEXTURE_H

#include "ZDintrin.h"

class ZDtexture {
protected:
	std::string name;
	std::string file_path;

	int_t width, height, channels;
	std::vector<color_t> data;

	void load_from(std::string);
public:
	ZDtexture(std::string, std::string);
};

#endif