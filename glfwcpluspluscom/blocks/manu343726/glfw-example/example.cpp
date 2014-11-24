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
#include <manu343726/glfw-example/ball.hpp>
#include <manu343726/glfw-example/triangle.hpp>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdexcept>

int main()
{
    auto app = make_app<ball>("bouncing ball!" , 800 , 600);
    
    app->start();
}

//! [code]
