#ifndef BALL_HPP
#define BALL_HPP

#include "glfw_app.hpp"

#include <utility>

class ball : public glfw_app
{
public:
	template<typename... ARGS>
	ball(ARGS&&... args) : glfw_app{ std::forward<ARGS>(args)... } , 
		x_ball{ 0.0f },
		y_ball{ 0.8f },
		vx_ball{ 0.0f },
		vy_ball{ 0.0f }
	{}

	virtual void on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods) override;

	virtual void glloop() override;

private:
	float x_ball, y_ball;
	float vx_ball, vy_ball;
	const float gravity = 0.0001;
	const float radious = 0.1f;

	void draw_ball();
};

#endif /* BALL_HPP */