#ifndef GLFW_APP_HPP
#define GLFW_APP_HPP

#include <string>
#include <diego\glfw\include\GLFW\glfw3.h>

class glfw_app
{
public:
	glfw_app(const std::string& window_title, int window_width, int window_height);
	virtual ~glfw_app();

	virtual void on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void on_error(int error, const char* desc)
	virtual void on_resize(GLFWwindow* window, int width, int height);
	virtual void glloop() = 0;

	std::pair<int, int> framebuffer_size() const;
	int framebuffer_width() const;
	int framebuffer_height() const;
	
private:
	GLFWwindow* _window;

	void glfwloop();
};

#endif /* GLFW_APP_HPP */