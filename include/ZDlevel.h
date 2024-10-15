#ifndef ZDLEVEL_H
#define ZDLEVEL_H

#include "Runtime.h"
#include "ZDcamera.h"

class ZDlevel {
protected:
	sycl::queue* queue;
	std::string file_path, name;

	std::vector<ZDmodel> host_models;
	std::vector<d_ZDmodel> device_models;
	std::vector<d_ZDinstance> device_instances;

	ZDcamera* camera;

	int_t find_host_model(std::string);

	void load_from(std::string);
public:
	ZDlevel(sycl::queue*, std::string, std::string);

	d_ZDmodel* get_device_models() { return this->device_models.data(); }
	d_ZDinstance* get_instances() { return this->device_instances.data(); }
	size_t get_model_count() { return this->device_models.size(); }
	size_t get_instance_count() { return this->device_instances.size(); }

	ZDcamera* get_camera() { return this->camera; }

	d_ZDmodel* models_to_gpu();
	d_ZDinstance* instances_to_gpu();
};

#endif
