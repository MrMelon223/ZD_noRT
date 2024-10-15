#ifndef COMPUTE_H
#define COMPUTE_H

#include "Helper.h"

class Compute {
protected:
	std::vector<sycl::device> devices;

	sycl::device main_cpu;
	sycl::device main_gpu;

	sycl::queue* cpu_queue;
	sycl::queue* gpu_queue;
public:
	Compute();

	sycl::queue* get_cpu_queue() { return this->cpu_queue; }
	sycl::queue* get_gpu_queue() { return this->gpu_queue; }

	void debug_print();
};

#endif
