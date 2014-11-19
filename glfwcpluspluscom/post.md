
# A simple OpenGL animation with glfw, step by step

*Written by [Manu Sánchez](http://manu343726.github.io).*

[glfw](http://www.glfw.org/) is C a window management library for OpenGL applications, a replacement of the old and well known GLUT and freeGLUT libraries.
The library is [actively maintained](https://github.com/glfw/glfw), and shipped with a great set of examples and docs.

In this article, we will learn how to setup a OpenGL application easily thanks to glfw, with a simple animation simulating a small bouncing ball.

## An overview of the glfw API

glfw is a C API which relies on callbacks to handle the different configurations, events, errors, etc an OpenGL application would need. 
Also the multiple resources you may use, such as windows, OpenGL contexts, etc are managed internally by the library, and it only provides you handles as identifiers of those resources.

``` c
GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
```

Here the `window` variable is just a handle to the window you requested with a call to `glfwCreateWindow()` function. You are not responsible of releasing the window resource manually, since its managed by the library. Of course you can, if you want to delete that window for whatever reason:

``` c
glfwDestroyWindow(window);
```

After that call, the `window` handle becomes invalid, and the window it represents is closed.


The point of this kind of design is that **the library manages the resources, you only use them**. So there are no resource leaks. You can customize the interaction with those resources via the callbacks the API provides. 

For example: What happens when my window is resized? I need to rearrange the viewport of my OpenGL render! Don't worry, you can say glfw what to do in that case, just setting up a callback:

``` c
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
```

## Our goal: A little, funny, squishy, and so cute bouncing ball


INSERT RESULT IMAGE HERE


## Setting up the window system

### Bring the window to life

### Pulling events from the outside world

### Setting up OpenGL

### Putting all together


## The animation

### Draw a ball

### Let's move it!

### Collision response: It should bounce!


## Summary

(biicode?)
