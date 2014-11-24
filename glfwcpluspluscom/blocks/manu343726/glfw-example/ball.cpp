
#include "ball.hpp"

#include <cmath>
#include <array>
#include <iostream>

void ball::on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void ball::draw_ball()
{
	const float full_angle = 2.0f*3.141592654f;
	float x, y;

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (std::size_t i = 0; i < 20; ++i)
	{
		x = x_ball + radious*(std::cos(i*full_angle / 20.0f));
		y = y_ball + radious*(std::sin(i*full_angle / 20.0f));

		glVertex2f(x, y);
	}

	glEnd();
}

void ball::glloop()
{
	float ratio = framebuffer_width() / (float)framebuffer_height();

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);

	//Bounce on window bottom
	if (y_ball - radious <= -1)
		vy_ball = std::abs(vy_ball);
	else
		vy_ball -= gravity; //Apply gravity

	//Update ball coordinates
	x_ball += vx_ball;
	y_ball += vy_ball;

	std::cout << y_ball + radious << std::endl;

	//Lets draw the ball!
	draw_ball();
}