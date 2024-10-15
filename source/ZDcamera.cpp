	// ZDcamera.cpp
#include "../include/ZDcamera.h"

ZDcamera::ZDcamera(int_t width, int_t height, float fov, vec3_t position, vec3_t direction) {

	this->width = width;
	this->height = height;

	this->position = position;
	this->direction = ZD::normalize(direction);

	this->rotation.x = atan2f(this->direction.y, this->direction.x);
	this->rotation.y = asinf(-this->direction.z);
	this->rotation.z = 0.0f;

	this->hori_fov = fov;
}

d_ZDcamera* ZDcamera::to_gpu(sycl::queue* queue) {
	d_ZDcamera cam{ queue, this->position, this->direction, this->rotation, this->hori_fov };

	d_ZDcamera* d_cam = sycl::malloc_device<d_ZDcamera>(1, *cam.queue);
	cam.queue->memcpy(d_cam, &cam, sizeof(d_ZDcamera));
	cam.queue->wait();

	return d_cam;
}

void ZDcamera::update_direction(float x, float y) {
	//printf("X,Y input mouse coord = {%.2f, %.2f}\n", rot.x, rot.y);
	float normalized_coord_x = ((x - (static_cast<float>(this->width) * 0.5f)) / static_cast<float>(this->width));
	float normalized_coord_y = ((y - (static_cast<float>(this->height) * 0.5f)) / static_cast<float>(this->height));
	//printf("X,Y normalized input mouse coord = {%.2f, %.2f}\n", normalized_coord_x, normalized_coord_y);

	float aspect_ratio = static_cast<float>(this->width) / static_cast<float>(this->height);

	float fov_hori_rad = this->hori_fov;
	float fov_vert_rad = this->hori_fov * (static_cast<float>(this->height) / static_cast<float>(this->width));
	float half_fov_hori_rad = fov_hori_rad * 0.5f;
	float half_fov_vert_rad = fov_vert_rad * 0.5f;

	float view_x = normalized_coord_x * half_fov_hori_rad * aspect_ratio;
	float view_y = normalized_coord_y * half_fov_vert_rad;

	this->rotation.x += view_x * 0.1f; //* (static_cast<float>(this->dims.x) / this->dims.y);
	this->rotation.y -= view_y * 0.1f;
	//this->rotation.z = 0.0f;

	if (this->rotation.y > 80.0f) {
		this->rotation.y = 80.0f;
	}
	if (this->rotation.y < -90.0f) {
		this->rotation.y = -90.0f;
	}

	float yaw = this->rotation.x * (PI / 180.0f),
		pitch = this->rotation.y * (PI / 180.0f);

	this->direction.x = sycl::cos(yaw) * sycl::cos(pitch);
	this->direction.y = sycl::sin(pitch);
	this->direction.z = sycl::sin(yaw) * sycl::cos(pitch);

	this->direction = ZD::normalize(this->direction);
}

void ZDcamera::forward(float t) {
	this->position = ZD::add_v3(this->position, ZD::multiply(ZD::normalize(this->direction), t));
}

void ZDcamera::backward(float t) {
	this->position = ZD::subtract_v3(this->position, ZD::multiply(ZD::normalize(this->direction), t));
}
void ZDcamera::right(float t) {
	this->position = ZD::add_v3(this->position, ZD::multiply( ZD::cross(this->direction, vec3_t{0.0f, 1.0f, 0.0f}), t));
}
void ZDcamera::left(float t) {
	this->position = ZD::subtract_v3(this->position, ZD::multiply(ZD::cross(this->direction, vec3_t{0.0f, 1.0f, 0.0f}), t));
}

void ZDcamera::debug_print() {
	std::cout << "Camera Debug:" << std::endl;
	std::cout << std::setw(15) << "Res: " << this->width << "x" << this->height << "p" << std::endl;
	std::cout << std::setw(15) << "Loc: { " << this->position.x << ", " << this->position.y << ", " << this->position.z << " }" << std::endl;
	std::cout << std::setw(15) << "Dir: { " << this->direction.x << ", " << this->direction.y << ", " << this->direction.z << " }" << std::endl;
}