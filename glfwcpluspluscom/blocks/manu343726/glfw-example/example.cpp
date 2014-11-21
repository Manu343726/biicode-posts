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
#include <manu343726/glfw-example/glfw_app.hpp>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdexcept>


class example_app : public glfw_app
{
public:
    template<typename... ARGS>
    example_app(ARGS&&... args) : glfw_app{ std::forward<ARGS>(args)... }
    {}
    
    virtual void on_keydown(GLFWwindow* window , int key , int scancode , int action , int mods) override
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);    
    }

    virtual void glloop() override
    {
        float ratio = framebuffer_width() / (float)framebuffer_height();
        
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
    }
};

int main()
{
    auto app = make_app<example_app>("hello!" , 800 , 600);
    
    app->start();
}

//! [code]
