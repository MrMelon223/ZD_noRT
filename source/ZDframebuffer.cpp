	// ZDframebuffer.cpp
#include "../include/ZDframebuffer.h"

void copy_color_buffer(d_ZDframebuffer* fb, color_t* c_buffer) {

	try {
		fb->queue->memcpy(c_buffer, fb->color_buffer, sizeof(color_t) * fb->width * fb->height);
		fb->queue->wait();
	}
	catch (sycl::exception& e) {
		std::cerr << "COPY_COLORBUFFER::ERROR: " << e.what() << std::endl;
	}
}

d_ZDframebuffer* create_framebuffer(sycl::queue* queue, int_t width, int_t height) {
	d_ZDframebuffer* buff = new d_ZDframebuffer{ nullptr, nullptr, nullptr, -1, -1 };

	buff->queue = queue;

	buff->width = width;
	buff->height = height;

	buff->color_buffer = sycl::malloc_device<color_t>(static_cast<size_t>(buff->width) * buff->height, *buff->queue);
	buff->depth_buffer = sycl::malloc_device<float>(static_cast<size_t>(buff->width) * buff->height, *buff->queue);

	buff->queue->wait();

	return buff;
}

void zero_buffers(d_ZDframebuffer* buff) {
	
	color_t* d_color_buff = buff->color_buffer;
	float* d_depth_buff = buff->depth_buffer;

	try {
		buff->queue->submit([&](sycl::handler& h) {
			h.parallel_for<class zero_buffer>(sycl::nd_range<1>{buff->width * buff->height, 1}, [=](sycl::nd_item<1> idx) {
				int index = idx.get_global_id(0);

				d_color_buff[index] = color_t{ 0.0f, 0.0f, 0.0f, 1.0f };
				d_depth_buff[index] = 0.0f;

			});
		});

		buff->queue->wait();
	}
	catch (sycl::exception& e) {
		std::cerr << "CLEAR_FRAMEBUFFER::ERROR: " << e.what() << std::endl;
	}
}

void cleanup_framebuffer(d_ZDframebuffer* buff) {
	sycl::free(buff->color_buffer, *buff->queue);
	sycl::free(buff->depth_buffer, *buff->queue);
}