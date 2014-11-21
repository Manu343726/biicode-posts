#ifndef GLFW_APP_HPP
#define GLFW_APP_HPP

#include <string>
#include <memory>
#include <diego/glfw/include/GLFW/glfw3.h>

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

    std::pair<int, int> framebuffer_size() const;
    int framebuffer_width() const;
    int framebuffer_height() const;
    
    GLFWwindow* window() const;

private:
    GLFWwindow* _window;

    void glfwloop();
};

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

template<typename T , typename... ARGS , typename = typename std::enable_if<std::is_base_of<glfw_app,T>::value>::type>
std::unique_ptr<T> make_app(ARGS&&... args)
{
    auto app = std::make_unique<T>( std::forward<ARGS>(args)...);
    
    glfw_app_manager::start_app(app.get());
    
    return app;
}


#endif /* GLFW_APP_HPP */