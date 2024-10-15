	// ZDlevel.cpp
#include "../include/ZDlevel.h"

int_t ZDlevel::find_host_model(std::string name) {
	int_t r;
	for (r = 0; r < static_cast<int_t>(this->host_models.size()); r++) {
		if (this->host_models[r].get_name() == name) {
			return r;
		}
	}
	std::cerr << "Cannot find ZDmodel: " << name << std::endl;
	return -1;
}

void ZDlevel::load_from(std::string path) {
	this->file_path = path;

	std::ifstream in;
	in.open(path, std::ios::in);
	if (!in) {
		std::cout << "Cannot find Level: " << path << std::endl;
		return;
	}

	std::cout << "Loading Level: " << path << std::endl;

	std::string line;
	std::getline(in, line);
	std::istringstream parse(line);
	size_t leng = 0;

	parse >> leng;
	std::cout << leng << " static models detected!" << std::endl;

	this->host_models = std::vector<ZDmodel>();
	this->device_models = std::vector<d_ZDmodel>();

	for (size_t i = 0; i < leng; i++) {
		std::getline(in, line);
		std::istringstream in0(line);

		float x, y, z, x_r, y_r, z_r, scale;
		std::string model;

		in0 >> x >> y >> z >> x_r >> y_r >> z_r >> scale >> model;
		//std::cout << model << std::endl;

		vec3_t position = vec3_t{ x, y, z };
		vec3_t rotation = vec3_t{ x_r, y_r, z_r };

		int_t h_idx = this->find_host_model(model);

		if (h_idx == -1) {
			int_t found_idx = ZDruntime::find_model_index(model);
			if (found_idx >= 0) {
				this->host_models.push_back(ZDruntime::HOST_MODELS[found_idx]);
				this->device_models.push_back(ZDruntime::HOST_MODELS[found_idx].to_gpu(this->queue));
				h_idx = static_cast<int_t>(this->host_models.size() - 1);
				std::cout << "Adding ZDmodel: " << ZDruntime::HOST_MODELS[found_idx].get_name() << " to ZDlevel Host Model Vector." << std::endl;
			}
			else {
				std::cerr << "Cannot Find Model: " << model << " in ZDruntime." << std::endl;
				continue;
			}
		}
		this->device_instances.push_back(create_instance(h_idx, position, rotation, this->host_models[h_idx].get_vertex_count(), this->host_models[h_idx].get_triangle_count(), true, scale, this->queue));

		std::cout << "d_model = " << this->device_instances.back().model_index << std::endl;
	}
}

ZDlevel::ZDlevel(sycl::queue* queue, std::string file_path, std::string name) {
	this->queue = queue;
	this->name = name;
	this->load_from(file_path);

	this->camera = new ZDcamera(1280, 720, 120.0f, vec3_t{ 0.0f, 0.0f, -1.0f }, vec3_t{ 0.0f, 0.0f, 1.0f });
}

d_ZDmodel* ZDlevel::models_to_gpu() {
	d_ZDmodel* models = sycl::malloc_device<d_ZDmodel>(this->device_models.size(), *this->queue);
	this->queue->memcpy(models, this->device_models.data(), sizeof(d_ZDmodel) * this->device_models.size());
	this->queue->wait();
	return models;
}

d_ZDinstance* ZDlevel::instances_to_gpu() {
	d_ZDinstance* instances = sycl::malloc_device<d_ZDinstance>(this->device_instances.size(), *this->queue);
	this->queue->memcpy(instances, this->device_instances.data(), sizeof(d_ZDinstance) * this->device_instances.size());
	this->queue->wait();
	return instances;
}