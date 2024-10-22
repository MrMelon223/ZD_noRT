	// ZDintrin.cpp
#include "../include/ZDintrin.h"

uv_t ZD::sum_uv(uv_t a, uv_t b) {
	return uv_t{ a.x + b.x, a.y + b.y };
}

vec4_t ZD::to_vec4(vec3_t a, float b) {
	return vec4_t{ a.x, a.y, a.z, b };
}

vec3_t ZD::to_vec3(vec4_t v) {
	return vec3_t{ v.x, v.y, v.z };
}

vec3_t ZD::subtract_v3(vec3_t a, vec3_t b) {
	return vec3_t{ a.x - b.x, a.y - b.y, a.z - b.z };
}

vec3_t ZD::add_v3(vec3_t a, vec3_t b) {
	return vec3_t{ a.x + b.x, a.y + b.y, a.z + b.z };
}

vec3_t ZD::product_m3(mat3_t m, vec3_t a) {
	return vec3_t{ a.x * m.data[0][0] + a.y * m.data[0][1] + a.z * m.data[0][2],
				a.x * m.data[1][0] + a.y * m.data[1][1] + a.z * m.data[1][2],
				a.x * m.data[2][0] + a.y * m.data[2][1] + a.z * m.data[2][2] };
}

vec4_t ZD::product_m4(mat4_t m, vec4_t a) {
	return vec4_t{ a.x * m.data[0][0] + a.y * m.data[1][0] + a.z * m.data[2][0] + a.w * m.data[3][0],
				a.x * m.data[0][1] + a.y * m.data[1][1] + a.z * m.data[2][1] + a.w * m.data[3][1],
				a.x * m.data[0][2] + a.y * m.data[1][2] + a.z * m.data[2][2] + a.w * m.data[3][2],
				a.x * m.data[0][3] + a.y * m.data[1][3] + a.z * m.data[2][3] + a.w * m.data[3][3]};
}

mat4_t ZD::create_m4(vec4_t a, vec4_t b, vec4_t c, vec4_t d) {
	mat4_t m{};
	m.data[0][0] = a.x;
	m.data[0][1] = b.x;
	m.data[0][2] = c.x;
	m.data[0][3] = d.x;

	m.data[1][0] = a.y;
	m.data[1][1] = b.y;
	m.data[1][2] = c.y;
	m.data[1][3] = d.y;

	m.data[2][0] = a.z;
	m.data[2][1] = b.z;
	m.data[2][2] = c.z;
	m.data[2][3] = d.z;

	m.data[3][0] = a.w;
	m.data[3][1] = b.w;
	m.data[3][2] = c.w;
	m.data[3][3] = d.w;

	return m;
}

color_t ZD::add(color_t a, color_t b) {
	return color_t{ a.x + b.x, a.y + b.y, a.z + b.z };
}

mat3_t ZD::create_m3(vec3_t a, vec3_t b, vec3_t c) {
	mat3_t m{};
	m.data[0][0] = a.x;
	m.data[0][1] = b.x;
	m.data[0][2] = c.x;

	m.data[1][0] = a.y;
	m.data[1][1] = b.y;
	m.data[1][2] = c.y;

	m.data[2][0] = a.z;
	m.data[2][1] = b.z;
	m.data[2][2] = c.z;

	return m;
}

vec3_t ZD::cross(vec3_t a, vec3_t b) {
	return vec3_t{ a.y * b.z - b.y * a.z,
				a.x * b.z - b.x * a.z,
				a.x * b.y - b.x * a.y };
}

float ZD::cross_2d(vec2_t p1, vec2_t p2, vec2_t p3) {
	return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

float ZD::dot(vec3_t a, vec3_t b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t ZD::normalize(vec3_t a) {
	float mag = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	return vec3_t{ a.x / mag, a.y / mag, a.z / mag };
}

float ZD::radians(float degrees) {
	return degrees * (PI / 180.0f);
}

mat4_t ZD::perspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
	float f = 1.0f / tanf(ZD::radians(fov) / 2.0f);
	float depth = nearPlane - farPlane;

	mat4_t matrix = ZD::create_m4(vec4_t{ 0.0f,0.0f,0.0f,0.0f }, 
		vec4_t{ 0.0f,0.0f,0.0f,0.0f }, 
		vec4_t{ 0.0f,0.0f,0.0f,0.0f }, 
		vec4_t{ 0.0f,0.0f,0.0f,0.0f });

	matrix.data[0][0] = f / aspectRatio;
	matrix.data[1][1] = f;
	matrix.data[2][2] = (farPlane + nearPlane) / depth;
	matrix.data[2][3] = (2 * farPlane * nearPlane) / depth;
	matrix.data[3][2] = -1.0f;
	matrix.data[3][3] = 0.0f;

	return matrix;
}

float ZD::minf(float a, float b) {
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

float ZD::maxf(float a, float b) {
	if (b > a) {
		return b;
	}
	else {
		return a;
	}

}

vec3_t ZD::min_v3(vec3_t a, vec3_t b) {
	vec3_t c = { ZD::minf(a.x, b.x), ZD::minf(a.y, b.y), ZD::minf(a.z, b.z) };
	return c;
}

vec3_t ZD::max_v3(vec3_t a, vec3_t b) {
	vec3_t c = { ZD::maxf(a.x, b.x), ZD::maxf(a.y, b.y), ZD::maxf(a.z, b.z) };
	return c;
}

vec3_t ZD::multiply(vec3_t v, float s) {
	return vec3_t{ v.x * s, v.y * s, v.z * s };
}

float ZD::signed_area(vec2_t p1, vec2_t p2, vec2_t p3) {
	return (p1.x * (p2.y - p3.y) +
		p2.x * (p3.y - p1.y) +
		p3.x * (p1.y - p2.y)) / 2.0f;
}

mat4_t ZD::rotate(vec3_t cameraRotation) {
	mat4_t rotationMatrix = {};  // Initialize to zero

	// Extract the yaw (rotation around the Y-axis) and pitch (rotation around the X-axis)
	float yaw = cameraRotation.y;   // Y-axis rotation (left/right)
	float pitch = cameraRotation.x; // X-axis rotation (up/down)

	// Compute cosines and sines
	float cosYaw = std::cos(yaw);
	float sinYaw = std::sin(yaw);
	float cosPitch = std::cos(pitch);
	float sinPitch = std::sin(pitch);

	// Combine pitch and yaw rotations into a single matrix (for FPV)
	rotationMatrix.data[0][0] = cosYaw;  // Yaw affects X
	rotationMatrix.data[0][1] = 0;
	rotationMatrix.data[0][2] = -sinYaw; // Yaw affects Z
	rotationMatrix.data[0][3] = 0;

	rotationMatrix.data[1][0] = sinYaw * sinPitch;  // Pitch affects Y
	rotationMatrix.data[1][1] = cosPitch;          // Pitch affects Y
	rotationMatrix.data[1][2] = cosYaw * sinPitch;
	rotationMatrix.data[1][3] = 0;

	rotationMatrix.data[2][0] = sinYaw * cosPitch;  // Combined Yaw/Pitch affects Z
	rotationMatrix.data[2][1] = -sinPitch;         // Pitch affects Y
	rotationMatrix.data[2][2] = cosYaw * cosPitch;
	rotationMatrix.data[2][3] = 0;

	// The last row remains [0, 0, 0, 1] for homogeneous coordinates
	rotationMatrix.data[3][0] = 0;
	rotationMatrix.data[3][1] = 0;
	rotationMatrix.data[3][2] = 0;
	rotationMatrix.data[3][3] = 1;

	return rotationMatrix;
}

mat4_t ZD::m_multiply(mat4_t matA, mat4_t matB) {
	mat4_t result = {};

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.data[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.data[i][j] += matA.data[i][k] * matB.data[k][j];
			}
		}
	}

	return result;
}

float ZD::line_equation(vec2_t p, vec2_t p1, vec2_t p2) {
	float A = p2.y - p1.y;
	float B = p1.x - p2.x;
	float C = p2.x * p1.y - p1.x * p2.y;
	return A * p.x + B * p.y + C;
}

mat4_t ZD::invert(mat4_t matrix, bool& failed) {
	mat4_t inverse{};
	uchar_t n = 4;

	// Augmenting the matrix with the identity matrix
	for (uchar_t i = 0; i < n; ++i) {
		for (uchar_t j = 0; j < n; ++j) {
			if (i == j) inverse.data[i][j] = 1;
			else inverse.data[i][j] = 0;
		}
	}

	for (uchar_t i = 0; i < n; ++i) {
		float pivot = matrix.data[i][i];
		/*if (pivot == 0) { // Non-invertible matrix
			failed = true;
			return inverse;
		}*/
		// Normalize the pivot row
		for (uchar_t j = 0; j < n; ++j) {
			matrix.data[i][j] /= pivot;
			inverse.data[i][j] /= pivot;
		}

		// Eliminate the other rows
		for (uchar_t k = 0; k < n; ++k) {
			if (k != i) {
				float factor = matrix.data[k][i];
				for (uchar_t j = 0; j < n; ++j) {
					matrix.data[k][j] -= factor * matrix.data[i][j];
					inverse.data[k][j] -= factor * inverse.data[i][j];
				}
			}
		}
	}
	failed = false;
	return inverse;
}