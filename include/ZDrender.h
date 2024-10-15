#ifndef ZDRENDER_H
#define ZDRENDER_H

#include "ZDlevel.h"

SYCL_EXTERNAL float line_equation(vec3_t, vec2_t);

void calculate_instance_visibility(d_ZDmodel*, d_ZDinstance*, d_ZDcamera*, int_t, sycl::queue*);

void draw(d_ZDframebuffer*, d_ZDmodel*, d_ZDinstance*, d_ZDcamera*, int_t, sycl::queue*);

#endif
