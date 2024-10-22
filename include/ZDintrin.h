#ifndef ZDINTRIN_H
#define ZDINTRIN_H

#include "Vectors.h"
#include "Helper.h"

namespace ZD {
	SYCL_EXTERNAL uv_t sum_uv(uv_t, uv_t);

	SYCL_EXTERNAL vec4_t to_vec4(vec3_t, float);
	SYCL_EXTERNAL vec3_t to_vec3(vec4_t);

	SYCL_EXTERNAL vec3_t subtract_v3(vec3_t, vec3_t);
	SYCL_EXTERNAL vec3_t add_v3(vec3_t, vec3_t);

	SYCL_EXTERNAL vec3_t product_m3(mat3_t, vec3_t);
	SYCL_EXTERNAL vec4_t product_m4(mat4_t, vec4_t);

	SYCL_EXTERNAL mat3_t create_m3(vec3_t, vec3_t, vec3_t); // Column Major
	SYCL_EXTERNAL mat4_t create_m4(vec4_t, vec4_t, vec4_t, vec4_t); // Column Major

	SYCL_EXTERNAL vec3_t cross(vec3_t, vec3_t);
	SYCL_EXTERNAL float cross_2d(vec2_t, vec2_t, vec2_t);
	SYCL_EXTERNAL float dot(vec3_t, vec3_t);

	SYCL_EXTERNAL vec3_t normalize(vec3_t);

	SYCL_EXTERNAL float radians(float);

	SYCL_EXTERNAL mat4_t perspective(float, float, float, float);

	SYCL_EXTERNAL float minf(float, float);
	SYCL_EXTERNAL float maxf(float, float);

	SYCL_EXTERNAL vec3_t min_v3(vec3_t, vec3_t);
	SYCL_EXTERNAL vec3_t max_v3(vec3_t, vec3_t);

	SYCL_EXTERNAL vec3_t multiply(vec3_t, float);

	SYCL_EXTERNAL float signed_area(vec2_t, vec2_t, vec2_t);

	SYCL_EXTERNAL mat4_t rotate(vec3_t);
	SYCL_EXTERNAL mat4_t m_multiply(mat4_t, mat4_t);

	SYCL_EXTERNAL float line_equation(vec2_t, vec2_t, vec2_t);

	SYCL_EXTERNAL mat4_t invert(mat4_t, bool&);
}

const float PI = 3.1415f;

#endif