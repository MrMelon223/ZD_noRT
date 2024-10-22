#ifndef ZDTEXTURE_H
#define ZDTEXTURE_H

#include "ZDintrin.h"

struct d_ZDtexture;

class ZDtexture {
protected:
	std::string name;
	std::string file_path;

	int_t width, height, channels;
	std::vector<color_t> data;

	void load_from(std::string);
public:
	ZDtexture(std::string, std::string);
	 
	d_ZDtexture to_gpu(sycl::queue*);
};

struct d_ZDtexture {
	sycl::queue* queue;
	int_t width, height;
	color_t* data;

	SYCL_EXTERNAL color_t sample(float, float);
};

#endif