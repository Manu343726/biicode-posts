//========================================================================
// Simple GLFW example
// Copyright (c) Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//! [code]

#include <diego/glfw/include/GLFW/glfw3.h>
#include <manu343726/scoped_resource/scoped_resouce.hpp>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdexcept>




int main()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    scoped_resource<GLFWwindow*> window
    {
    	[](GLFWwindow*& window_handle)
    	{
    		window_handle = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    		if(!window_handle) throw std::runtime_error{"Error creating window"};
    	},
    	[](GLFWwindow*& window_handle)
    	{
    		glfwDestroyWindow(window_handle);
    	}
    };

    glfwSetErrorCallback([](int error , const char* desc)
    {
    	std::cerr << "ERROR: " << desc << std::endl;		
    });

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);    		
    });

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);

        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

        glBegin(GL_TRIANGLES);
	        glColor3f(1.f, 0.f, 0.f);
	        glVertex3f(-0.6f, -0.4f, 0.f);
	        glColor3f(0.f, 1.f, 0.f);
	        glVertex3f(0.6f, -0.4f, 0.f);
	        glColor3f(0.f, 0.f, 1.f);
	        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

//! [code]
