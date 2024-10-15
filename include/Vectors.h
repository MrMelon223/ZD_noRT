#ifndef VECTORS_H
#define VECTORS_H

#include <cstdint>

typedef uint8_t uchar_t;
typedef int8_t char_t;

typedef uint16_t ushort_t;
typedef int16_t short_t;

typedef uint32_t uint_t;
typedef int32_t int_t;

typedef uint64_t ulong_t;
typedef int64_t long_t;

struct vec2_t {
	float x, y;
};

typedef vec2_t uv_t;

struct vec3_t {
	float x, y, z;
};

struct vec4_t {
	float x, y, z, w;
};

struct tri_t {
	uint_t a, b, c;
};

typedef vec4_t color_t;

struct mat3_t {
	float data[3][3];
};

struct mat4_t {
	float data[4][4];
};

struct ivec2_t {
	int_t x, y;
};

#endif
