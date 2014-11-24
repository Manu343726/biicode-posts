#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "glfw_app.hpp"

class triangle : public glfw_app
{
public:
	template<typename... ARGS>
	triangle(ARGS&&... args) : glfw_app{ std::forward<ARGS>(args)... }
	{}

	virtual void on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods) override;

	virtual void glloop() override;
};

#endif /* TRIANGLE_HPP */