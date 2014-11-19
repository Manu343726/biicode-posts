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

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <functional>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

template<typename T>
struct scoped_resource
{
public:
    using initializer_t = void(T&);
    using releaser_t = void(T&);
    
private:
    std::function<void(T&)> _releaser;
    T _resource;
    
public:
    template<typename... ARGS , typename R>
    scoped_resource(initializer_t init , R release , ARGS&&... args) : 
        _releaser{ release },
        _resource{ std::forward<ARGS>(args)... }      
    {
        init(_resource);
    }

    ~scoped_resource()
    {
        _releaser(_resource);
    }

    const T& get() const
    {
        return _resource;
    }

    T& get()
    {
        return _resource;
    }

    operator const T&() const
    {
        return get();
    }

    operator T&()
    {
        return get();
    }
};

int main(void)
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
