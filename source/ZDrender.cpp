	// ZDrender.cpp
#include "../include/ZDrender.h"

class cull_models_from_camera;
class draw_instances;

void ZDrender::calculate_instance_visibility(d_ZDmodel* models, d_ZDinstance* instances, int_t instance_count, d_ZDcamera* camera, sycl::queue* queue) {
	try {
		d_ZDmodel* d_models = models;
		d_ZDinstance* d_instances = instances;
		d_ZDcamera* d_camera = camera;

		queue->submit([&](sycl::handler& h) {
			h.parallel_for<class cull_models_from_camera>(sycl::nd_range<1>{instance_count, 1}, [=](sycl::nd_item<1> idx) {
				int index = idx.get_global_id(0);

				d_ZDinstance* inst = &d_instances[index];
				d_ZDmodel* model = &d_models[inst->model_index];

				vec3_t cam_to_inst = ZD::subtract_v3(d_camera->position, inst->position);
				float d_prod = ZD::dot(cam_to_inst, d_camera->direction);
				inst->show = true;
				if (d_prod <= 0.0f) {
					inst->show = false;
				}
				else {
					cam_to_inst = ZD::normalize(cam_to_inst);
					float angle = ZD::dot(d_camera->direction, cam_to_inst);
					if (angle >= sycl::cos(d_camera->hori_fov * 0.5f)) {
						inst->show = true;
					}
					else {
						inst->show = false;
					}
				}

			});
		});

		queue->wait();
	}
	catch (sycl::exception& e) {
		std::cerr << "CULL_MODELS::ERROR: " << e.what() << std::endl;
	}
}

void ZDrender::draw(d_ZDframebuffer* buff, d_ZDmodel* models, d_ZDinstance* instances, d_ZDcamera* camera, int_t instance_count, sycl::queue* queue) {
	color_t* d_color_buff = buff->color_buffer;
	float* d_depth_buff = buff->depth_buffer;
	d_ZDmodel* d_models = models;
	d_ZDinstance* d_instances = instances;
	d_ZDcamera* d_camera = camera;
	int_t* d_width, * d_height, * d_instance_count;

	try {
		d_width = sycl::malloc_device<int_t>(1, *queue),
		d_height = sycl::malloc_device<int_t>(1, *queue);
		d_instance_count = sycl::malloc_device<int_t>(1, *queue);
		queue->memcpy(d_width, &buff->width, sizeof(int_t));
		queue->memcpy(d_height, &buff->height, sizeof(int_t));
		queue->memcpy(d_instance_count, &instance_count, sizeof(int_t));
		queue->wait();
	}
	catch (sycl::exception& e) {
		std::cerr << "COPY_CAM_DATA::ERROR: " << e.what() << std::endl;
	}

	try {
		printf("Transforming %i instances.\n", instance_count);
		queue->submit([&](sycl::handler& h) {
			h.parallel_for<class draw_instances>(sycl::nd_range<1>{instance_count, 1}, [=](sycl::nd_item<1> idx) {
				int index = idx.get_global_id(0);

				d_ZDinstance* inst = &d_instances[index];
				d_ZDmodel* model = &d_models[inst->model_index];

				vec3_t* positions = model->vertex_positions;
				vec3_t* normals = model->triangle_normals;
				tri_t* tri_idxs = model->triangle_indices;
					// float fov, float aspectRatio, float nearPlane, float farPlane
				mat4_t perspect = ZD::perspective(d_camera->hori_fov, static_cast<float>(*d_width) / *d_height, 0.01f, 100.0f);
				bool fail_invert = false;
				mat4_t cam_mtx = ZD::invert(ZD::rotate(d_camera->rotation), fail_invert);
				mat4_t rotation = ZD::rotate(inst->rotation);

				for (uint_t i = 0; i < inst->vertex_count; i++) {
						//if (inst->visible_triangles[i]) {
						vec3_t v0 = positions[i];
						vec3_t n0 = 
						v0 = ZD::add_v3(v0, ZD::subtract_v3(d_camera->position, inst->position));

						v0 = ZD::to_vec3(ZD::product_m4(rotation, ZD::to_vec4(v0, 0.0f)));
						v0 = ZD::to_vec3(ZD::product_m4(cam_mtx, ZD::to_vec4(v0, 0.0f)));
						v0 = ZD::to_vec3(ZD::product_m4(perspect, ZD::to_vec4(v0, 0.0f)));


						inst->transformed_vertices[i] = vec3_t{ inst->scale * v0.x, inst->scale * v0.y, inst->scale * v0.z };
				}
				for (uint_t i = 0; i < inst->triangle_count; i++) {
					vec3_t v0 = normals[i];
					//v0 = ZD::add_v3(v0, ZD::subtract_v3(d_camera->position, inst->position));

					v0 = ZD::to_vec3(ZD::product_m4(rotation, ZD::to_vec4(v0, 0.0f)));
					v0 = ZD::to_vec3(ZD::product_m4(cam_mtx, ZD::to_vec4(v0, 0.0f)));


					inst->transformed_normals[i] = v0;
				}
			});
		});

		queue->wait();
	}
	catch (sycl::exception& e) {
		std::cerr << "DRAW_INSTANCES::ERROR: " << e.what() << std::endl;
	}

	try {
		queue->submit([&](sycl::handler& h) {
			h.parallel_for<class interpolate_instances>(sycl::nd_range<1>{buff->height * buff->width, 1}, [=](sycl::nd_item<1> idx) {
				int index = idx.get_global_id(0);

				int_t x = index % *d_width,
					y = (index - x) / *d_width;
				vec2_t normalized_coord{};

				float ratio = static_cast<float>(*d_width) / static_cast<float>(*d_height);
				float norm_x = (static_cast<float>(x) - (static_cast<float>(*d_width) * 0.5f)) / (static_cast<float>(*d_width));
				float norm_y = (static_cast<float>(y) - (static_cast<float>(*d_height) * 0.5f)) / (static_cast<float>(*d_height));
				float fov_rad = d_camera->hori_fov * (PI / 180.0f);
				float half_fov = fov_rad * 0.5f;

				normalized_coord.x = norm_x * ratio;
				normalized_coord.y = norm_y;

				d_color_buff[y * *d_width + x] = color_t{ 0.17f, 0.15f, 0.17f, 1.0f };

				for (uint_t i = 0; i < *d_instance_count; i++) {
					d_ZDinstance* instance = &d_instances[i];
					if (instance->show) {
						d_ZDmodel* model = &d_models[instance->model_index];
						for (uint_t j = 0; j < instance->triangle_count; j++) {
							//if (instance->visible_triangles[j]) {
								tri_t t = d_models[instance->model_index].triangle_indices[j];
								vec3_t normal = instance->transformed_normals[j];

								float n_dot = ZD::dot(normal, d_camera->direction);
								//if (n_dot <= 0.0f) {

									vec3_t v0 = instance->transformed_vertices[t.a],
										v1 = instance->transformed_vertices[t.b],
										v2 = instance->transformed_vertices[t.c];

									vec2_t v0a = { -v0.x / v0.z, -v0.y / v0.z },
										v1a = { -v1.x / v1.z, -v1.y / v1.z },
										v2a = { -v2.x / v2.z, -v2.y / v2.z };

									float sign1 = ZD::line_equation(normalized_coord, v0a, v1a),
										sign2 = ZD::line_equation(normalized_coord, v1a, v2a),
										sign3 = ZD::line_equation(normalized_coord, v2a, v0a);


									if ((sign1 >= 0.0f && sign2 >= 0.0f && sign3 >= 0.0f) ||
										(sign1 <= 0.0f && sign2 <= 0.0f && sign3 <= 0.0f)) {
										d_color_buff[y * *d_width + x] = color_t{ 1.0f, 1.0f, 1.0f, 1.0f };
									}
								//}
							//}
						}
					}
				}
			});
		});

		queue->wait();
	}
	catch (sycl::exception& e) {
		std::cerr << "INTERPOLATE_VERTICES::ERROR: " << e.what() << std::endl;
	}

	sycl::free(d_width, *queue);
	sycl::free(d_height, *queue);
	sycl::free(d_instance_count, *queue);
}