#ifndef ZDCAMERA_H
#define ZDCAMERA_H

#include "ZDframebuffer.h"

struct d_ZDcamera;
struct d_ZDfrustrum;

class ZDcamera {
protected:
	int_t width, height;
	vec3_t position, direction, rotation;

	float hori_fov;
public:
	ZDcamera(int_t, int_t, float, vec3_t, vec3_t);

	void set_direction(vec3_t d) { this->direction = d; }

	vec3_t get_position() { return this->position; }

	void update_direction(float, float);

	void forward(float);
	void backward(float);
	void right(float);
	void left(float);

	void turn_right(float);
	void turn_left(float);

	void debug_print();

	d_ZDcamera* to_gpu(sycl::queue*);
};

struct d_ZDcamera {
	sycl::queue* queue;
	vec3_t position, direction, rotation;
	float hori_fov;
};

#endif