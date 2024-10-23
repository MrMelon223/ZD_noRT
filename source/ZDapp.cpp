	// ZDapp.cpp
#include "../include/ZDapp.h"

std::queue<KeyboardButtonUse> keyboard_button_uses;
std::queue<MouseButtonUse> mouse_button_uses;

static void keyboard_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {

	KeyboardButtonUse k{};

	k.key = key;
	k.scancode = scancode;
	k.action = action;
	k.mods = mods;

	keyboard_button_uses.push(k);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {

	MouseButtonUse k{};

	k.button = button;
	k.action = action;
	k.mods = mods;

	mouse_button_uses.push(k);
}

void ZDapp::empty_queues() {
	while (!keyboard_button_uses.empty()) {
		this->input_handle(keyboard_button_uses.front());
		keyboard_button_uses.pop();
	}

	while (!mouse_button_uses.empty()) {
		this->mouse_handle(mouse_button_uses.front());
		mouse_button_uses.pop();
	}
}

void ZDapp::input_handle(KeyboardButtonUse& k) {

	int CURRENT_KEY = k.key;
	int CURRENT_ACTION = k.action;

	std::cout << CURRENT_KEY << std::endl;

	//this->is_walking = false, this->is_sprinting = false, this->trying_sprint = false;;
	switch (CURRENT_KEY) {
	case GLFW_KEY_RIGHT:
		if (CURRENT_ACTION == GLFW_RELEASE) {

		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->current_level->get_camera()->turn_right(glfwGetTime() - this->last_time);
			this->current_level->get_camera()->debug_print();
		}
		break;
	case GLFW_KEY_LEFT:
		if (CURRENT_ACTION == GLFW_RELEASE) {

		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->current_level->get_camera()->turn_left(glfwGetTime() - this->last_time);
			this->current_level->get_camera()->debug_print();
		}
		break;
	case GLFW_KEY_UP:
		if (CURRENT_ACTION == GLFW_RELEASE) {

		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->current_level->get_camera()->turn_up(glfwGetTime() - this->last_time);
			this->current_level->get_camera()->debug_print();
		}
		break;
	case GLFW_KEY_DOWN:
		if (CURRENT_ACTION == GLFW_RELEASE) {

		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->current_level->get_camera()->turn_down(glfwGetTime() - this->last_time);
			this->current_level->get_camera()->debug_print();
		}
		break;
	case GLFW_KEY_D:
		if (CURRENT_ACTION == GLFW_RELEASE) {

		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->current_level->get_camera()->forward(glfwGetTime() - this->last_time);
			this->current_level->get_camera()->debug_print();
		}
		break;
	case GLFW_KEY_A:
		if (CURRENT_ACTION == GLFW_RELEASE) {

		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->current_level->get_camera()->backward(glfwGetTime() - this->last_time);
			this->current_level->get_camera()->debug_print();
		}
		break;
	case GLFW_KEY_S:
		if (CURRENT_ACTION == GLFW_RELEASE) {

		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->current_level->get_camera()->right(glfwGetTime() - this->last_time);
			this->current_level->get_camera()->debug_print();
		}
		break;
	case GLFW_KEY_W:
		if (CURRENT_ACTION == GLFW_RELEASE) {

		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->current_level->get_camera()->left(glfwGetTime() - this->last_time);
			this->current_level->get_camera()->debug_print();
		}
		break;
	case GLFW_KEY_R:
		if (CURRENT_ACTION == GLFW_RELEASE) {
		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			//this->cam->set_capture_mode(RT);
		}
		if (CURRENT_ACTION == GLFW_RELEASE) {
		}
		break;
	case GLFW_KEY_F:
		if (CURRENT_ACTION == GLFW_RELEASE) {
		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			//this->camera->set_capture_mode(FULLBRIGHT);
		}
		break;
	case GLFW_KEY_ESCAPE:
		if (CURRENT_ACTION == GLFW_RELEASE) {
		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			this->loop = false;
		}
		break;
	case GLFW_KEY_P:
		if (CURRENT_ACTION == GLFW_RELEASE) {
		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {
			//this->cam->get_rays(0)->debug_stats();
		}
		break;
	case GLFW_KEY_SPACE:
		if (CURRENT_ACTION == GLFW_RELEASE) {
		}
		if (CURRENT_ACTION == GLFW_PRESS || CURRENT_ACTION == GLFW_REPEAT) {

		}
		break;
	case GLFW_MOUSE_BUTTON_LEFT:
		if (CURRENT_ACTION == GLFW_RELEASE) {
		}
		if (CURRENT_ACTION == GLFW_REPEAT) {

		}
		break;
	}

	int CURRENT_MOD = k.mods;
	switch (CURRENT_MOD) {
	case GLFW_MOD_SHIFT:

		break;
	}
}

void ZDapp::mouse_handle(MouseButtonUse& m) {

}

void ZDapp::setup_gamepad() {
	this->gamepad = false;
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (present == GLFW_TRUE) {
		this->gamepad = true;
		return;
	}
}

ZDapp::ZDapp(int_t width, int_t height) {
	this->compute = new Compute();
	this->compute->debug_print();

	ZDruntime::load_textures();
	ZDruntime::load_models();

	this->width = width;
	this->height = height;

	this->current_level = new ZDlevel(this->compute->get_gpu_queue(), "resources/levels/test_level.txt", "test_level");
}

void ZDapp::main_loop() {
	if (glfwInit() == -1) {
		std::cerr << "Failed to initialize GLFW\n";
		return;
	}

	this->window = glfwCreateWindow(this->width, this->height, "ZDnoRT", NULL, NULL);
	glfwSetKeyCallback(this->window, keyboard_callback);
	glfwSetMouseButtonCallback(this->window, mouse_callback);

	this->loop = true;

	d_ZDmodel* d_models = this->current_level->models_to_gpu();
	d_ZDinstance* d_instances = this->current_level->instances_to_gpu();
	d_ZDtexture* d_textures = this->current_level->textures_to_gpu();
	this->frame_buffer = create_framebuffer(this->compute->get_gpu_queue(), this->width, this->height);
	size_t frame_count = 0;
	this->last_time = glfwGetTime();
	d_ZDcamera* d_cam;
	d_ZDvertex_sample* sample_ptr = nullptr;
	ZDcamera* camera = this->current_level->get_camera();
	this->setup_gamepad();

	sycl::queue* gpu = this->compute->get_gpu_queue();
	while (this->loop && !glfwWindowShouldClose(this->window)) {
		glfwMakeContextCurrent(this->window);
		//zero_buffers(this->frame_buffer);

		color_t* color_buff = new color_t[this->frame_buffer->width * this->frame_buffer->height];

		d_cam = this->current_level->get_camera()->to_gpu(this->compute->get_gpu_queue());

		ZDrender::calculate_instance_visibility(d_models, d_instances, static_cast<int_t>(this->current_level->get_instance_count()), d_cam, gpu);
		ZDrender::draw(this->frame_buffer, d_models, d_textures, d_instances, d_cam, static_cast<int_t>(this->current_level->get_instance_count()), gpu);

		copy_color_buffer(this->frame_buffer, color_buff);

		/*for (int_t y = 0; y < this->height; y++) {
			for (int_t x = 0; x < this->width; x++) {
				color_buff[y * this->width + x] = color_t{ 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}*/

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(this->width, this->height, GL_BGRA_EXT, GL_FLOAT, color_buff);
		glfwSwapBuffers(this->window);
		glfwPollEvents();

		double x, y;
		if (this->gamepad) {
			int count;
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

			camera->turn_right_for(axes[2]);
			camera->look_up_for(axes[3]);
			
			camera->forward(axes[0]);
			camera->right(axes[1]);
		}
		else {
			glfwGetCursorPos(this->window, &x, &y);
			glfwSetCursorPos(this->window, 0.5 * static_cast<double>(this->width), 0.5 * static_cast<double>(this->height));
			this->current_level->get_camera()->update_direction(static_cast<float>(x), static_cast<float>(y));
		}

		this->current_level->get_camera()->update_direction();

		this->empty_queues();

		delete[] color_buff;
		std::cout << "Frame " << frame_count << " Complete." << std::endl;
		frame_count++;
		this->last_time = glfwGetTime() - this->last_time;
		sample_ptr = from_gpu(d_cam, this->compute->get_gpu_queue());
		sycl::free(sample_ptr, *this->compute->get_gpu_queue());
		sycl::free(d_cam, *this->compute->get_gpu_queue());
	}

	cleanup_framebuffer(this->frame_buffer);

	sycl::free(d_instances, *this->compute->get_gpu_queue());
}