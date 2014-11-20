
#include "glfw_app.hpp"

glfw_app::glfw_app(const std::string& window_title, int window_width, int window_height)
{
	if (!glfwInit())
		throw std::runtime_error{ "Unable to initialize glfw runtime" };

	_window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);

	if (!_window)
		throw std::runtime_error{ "Unable to initialize glfw window" };

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);

	glfwloop();
}

glfw_app::~glfw_app()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void glfw_app::glfwloop()
{
	while (!glfwWindowShouldClose(_window))
	{
		this->glloop();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void glfw_app::on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Does nothing by default. Override to customize
}

void on_resize(GLFWwindow* window, int width, int height)
{
	//By defualt rearranges OpenGL viewport to the current framebuffer size.

	glViewport(width, height);
}

std::pair<int, int> glfw_app::framebuffer_size() const
{
	std::pair<int, int> size;

	glfwGetFramebufferSize(_window, &size.first, &size.second);

	return size; //Happy RVO folks
}

int glfw_app::framebuffer_width() const
{
	return framebuffer_size().first;
}

int glfw_app::framebuffer_height() const
{
	return framebuffer_size().second;
}