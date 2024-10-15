	// Compute.cpp
#include "../include/Compute.h"

Compute::Compute() {

	this->main_gpu = sycl::device(sycl::gpu_selector_v);
	this->main_cpu = sycl::device(sycl::cpu_selector_v);

	for (int i = 0; i < sycl::platform::get_platforms().size(); i++) {
		auto p = sycl::platform::get_platforms().at(i);
		for (auto d : p.get_devices()) {
			if (d.is_gpu()) {
				if (this->main_gpu.get_info<sycl::info::device::max_compute_units>() <= d.get_info<sycl::info::device::max_compute_units>() && i > 1) {
					this->main_gpu = d;
					std::cout << p.get_info<sycl::info::platform::name>() << std::endl;
				}
			}
			if (this->main_cpu.get_info<sycl::info::device::max_compute_units>() <= d.get_info<sycl::info::device::max_compute_units>() && d.is_cpu() && i < 1) {
				this->main_cpu = d;
				std::cout << p.get_info<sycl::info::platform::name>() << std::endl;
			}
		}
	}

	this->cpu_queue = new sycl::queue(this->main_cpu);
	this->gpu_queue = new sycl::queue(this->main_gpu);
}

void Compute::debug_print() {
	std::cout << std::setw(20) << "Main CPU: " << this->main_cpu.get_info<sycl::info::device::name>() << std::endl;
	std::cout << std::setw(20) << "Main GPU: " << this->main_gpu.get_info<sycl::info::device::name>() << std::endl;
}