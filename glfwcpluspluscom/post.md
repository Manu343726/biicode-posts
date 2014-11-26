
# A simple OpenGL animation with glfw, step by step

*Written by [Manu Sánchez](http://manu343726.github.io).*

[glfw](http://www.glfw.org/) is C a window management library for OpenGL applications, a replacement of the old and well known GLUT and freeGLUT libraries.
The library is [actively maintained](https://github.com/glfw/glfw), and shipped with a great set of examples and docs.

In this article, we will learn how to setup a OpenGL application easily thanks to glfw, with a simple animation simulating a small bouncing ball.

## An overview of the glfw API

glfw is a C API which relies on callbacks to handle the different configurations, events, errors, etc an OpenGL application would need. 
Also the multiple resources you may use, such as windows, OpenGL contexts, etc. are managed internally by the library, and it only provides you handles as identifiers of those resources.

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

Let's write a simple animation of a white bouncing ball. I'm not a game designer, the goal here is to have the animation working with a couple of lines only.   

Apologizes in advance to anyone who's eyes hurt after watching this image:

![](http://s27.postimg.org/89p12gptf/Captura.png)  
*As I said, I'm a programmer...*

## A glfw application with C++
glfw has a C API. That's fine, but I'm a C++ programmer. Let's wrap this API in a simple inheritance-based little framework.

### The `glfw_app` base class

What I propose is a simple design to delegate all the repetitive tasks on the base class, and then create a custom glfw-based OpenGL application in a simple way, just inheriting and customizing what you need via polymorphism.

``` cpp
class glfw_app 
{
public:
    glfw_app(const std::string& window_title, int window_width, int window_height);
    virtual ~glfw_app();
    
    void start();

    virtual void on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods);
    virtual void on_error(int error, const char* desc);
    virtual void on_resize(GLFWwindow* window, int width, int height);
    virtual void glloop() = 0;
    
    GLFWwindow* window() const;
};
```

This base class is simple: It manages a glfw window and their OpenGL context for us, wraps (and currently hides) the event and rendering loop, finally and provides us some polymorphic functions to say what to do when a key is pressed, when the window is resized, etc. 

Take the most simple glfw example, a simple triangle (Extracted from [glfw docs](http://www.glfw.org/docs/latest/quick.html#quick_example)). It can be written with a couple of lines thanks to our `glfw_class` class:

``` cpp
void triangle::on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void triangle::glloop()
{
	float ratio = glfw_app::framebuffer_width() / (float)glfw_app::framebuffer_height();

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
}
```

That's all! All the other things (Buffer swapping, window and gl context management, etc.) is done by the base class. How? Let's see it step by step.

#### Resource management

As we have seen above, the `glfw_app` class is designed to manage one glfw window and its corresponding OpenGl setup. In that way, all the glfw/OpenGL setup is done in the constructor of the class, and all the cleanup on the destructor:

``` cpp
glfw_app::glfw_app(const std::string& window_title , int window_width , int window_height)
{
    if( !glfwInit() )
        throw std::runtime_error
    {
        "Unable to initialize glfw runtime"
    };

    _window = glfwCreateWindow(window_width , window_height , window_title.c_str() , nullptr , nullptr);

    if( !_window )
        throw std::runtime_error
    {
        "Unable to initialize glfw window"
    };

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
}

glfw_app::~glfw_app()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}
```

The class acts as a singleton: There is only one `glfw_app` instance per application since there's only one glfw application (The app itself).

#### Main loop

The main loop is encapsulated. This makes it simpler to write a custom OpenGL application, since in most of the cases this loop is almost the same (Fetch events, render, swap buffers):

``` cpp
void glfw_app::start()
{
    glfwloop();
}

void glfw_app::glfwloop()
{
    while( !glfwWindowShouldClose(_window) )
    {
	    //Here we call our custom loop body
        this->glloop(); 

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}
```

#### Event handling

`glfw_app` has some polymorphic functions for event handling of the form *`on_EVENT()`*. They just wrap the original glfw callbacks, but customization via polymorphism is more natural to OOP programmers:

``` cpp
void glfw_app::on_keydown(GLFWwindow* window , int key , int scancode , int action , int mods) 
{
    //Does nothing by default. Override to customize
}

void glfw_app::on_error(int error , const char* desc) 
{
    //Does nothing by default
}

void glfw_app::on_resize(GLFWwindow* window , int width , int height)
{
    //By defualt rearranges OpenGL viewport to the current framebuffer size.

    glViewport(0 , 0 , width , height);
}
```

### Callback API vs OOP

Is not that easy. We cannot just pass polymorphic functions to C callbacks, since they are not convertible to plain function objects. That makes sense, since (even ignoring the dynamic dispatching part) they need an object to be called with.

To be able to inject these polymorphic functions as callbacks for the glfw API, we need a bridge between the C and the C++ world. `static` member functions!

``` cpp
class glfw_app_manager
{
    static glfw_app* _app;
    
    static void on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(_app) _app->on_keydown(window,key,scancode,action,mods);
    }
    
    static void on_error(int error, const char* desc)
    {
        if(_app) _app->on_error(error,desc);
    }
    
    static void on_resize(GLFWwindow* window, int width, int height)
    {
        if(_app) _app->on_resize(window,width,height);
    }
    
public:
    static void start_app(glfw_app* app)
    {
        _app = app;
        
        glfwSetKeyCallback(app->window() , on_keydown);
        glfwSetFramebufferSizeCallback(app->window() , on_resize);
        glfwSetErrorCallback(on_error);
    }
};
```

As I said previously, our app class is actually a singleton. The `glfw_app_manager` class is the one managing it. Stores the current app instance, registers our bridges as callbacks, and then calls our app functions on them.

Finally, put a bit of dressing to our little framework by writing a function template to easy instance glfw applications:

``` cpp
template<typename T , typename... ARGS , typename = typename std::enable_if<std::is_base_of<glfw_app,T>::value>::type>
std::unique_ptr<T> make_app(ARGS&&... args)
{
    auto app = std::make_unique<T>( std::forward<ARGS>(args)...);
    
    glfw_app_manager::start_app(app.get());
    
    return app;
}
```

Using that, setting up a glfw application can be as easy as:

``` cpp

#include "glfw_app.hpp"
#include "your_glfw_app.hpp"

int main()
{
    auto app = make_app<your_glfw_app>("glfw!" , 800 , 600);
    
    app->start();
}
```

## TL;DR. Show me the ball!

Here's the declaration of the bouncing ball glfw application:

``` cpp
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
	const float gravity = 0.01;
	const float radius = 0.05f;

	void draw_ball();
};
```

We have ball coordinates, ball speed, and its radius. There is also a `gravity` constant, since we want our ball to bounce.  

The `on_keydon()` callback is not complex: Just closes the window when the user presses `ESC`:

``` cpp
void ball::on_keydown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
```

Now let's see the body of our rendering loop:

``` cpp
void ball::glloop()
{
	float ratio = framebuffer_width() / (float)framebuffer_height();

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);

	//Bounce on window bottom
	if (y_ball + radious <= radious)
		vy_ball = std::abs(vy_ball);
	else
		vy_ball -= gravity; //Apply gravity

	//Update ball coordinates
	x_ball += vx_ball;
	y_ball += vy_ball;

	//Lets draw the ball!
	draw_ball();
}
```

Note how the ball is projected. The visible area of our OpenGL scene (The area which matches the viewport) goes from -1 to 1 in both axes, where -1 is the bottom-left corner of our window, and 1 is its top-left.  
Working with coordinates [-1,1] makes it simple to deal with window bounds, since they are independent of the window's size.

Check how the animation works:

``` cpp
	//Bounce on window bottom
	if (y_ball - radious <= - 1)
		vy_ball = std::abs(vy_ball);
	else
		vy_ball -= gravity; //Apply gravity

	//Update ball coordinates
	x_ball += vx_ball;
	y_ball += vy_ball;

	//Lets draw the ball!
	draw_ball();
```

The ball's position and speed are updated following the equations `v' = v + a*t` and `p' = p + v * t`, where `v` is velocity (speed), `a` is acceleration (The `gravity` constant), and `t` is time. 

Time is measured in frames, so in all the equations `t` is one. That's why there's no `t` in our code. *If you want a stable simulation (independent of frame rate) you should use a more complex technique, like those described in [this article](http://gafferongames.com/game-physics/fix-your-timestep/).*

If the ball goes out of the window bounds, that is, `y_ball - radious` is less than -1, we should make the ball go upwards: Set its vertical speed as positive:

``` cpp
if (y_ball - radious <= - 1)
		vy_ball = std::abs(vy_ball);
```

Also apply gravity. Don't apply acceleration when the ball bounces. 

The final step is to draw the ball: Draw a white "circle" (a regular polygon) using `GL_POLYGON`:

``` cpp
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
```

That's all! Now start our ball app:

``` cpp

#include "glfw_app.hpp"
#include "ball.hpp"

int main()
{
    auto app = make_app<ball>("bouncing ball!" , 800 , 600);
    
    app->start();
}
```

## Summary

glfw is a great library to write OpenGL applications. Its C API is clear and simple, and making it work in  the C++ way can be done with just a little effort.   
We learnt here how to make a little framework to write simple OpenGL applications in a OO way. Encapsulating the most common tasks in a base class reduces noise in our simple OpenGL examples.

All the code shown here is available at [biicode](https://www.biicode.com/), a dependency manager for C and C++. Just open the block with `$ bii open manu343726/glfw-examples` and configure it to use your preferred C++ compiler.
