//
// Created by jackjwang on 2021/9/12.
//

#ifndef WJ_PLAYGROUND_PTAPPWINDOW_H
#define WJ_PLAYGROUND_PTAPPWINDOW_H

#define GLFW_INCLUDE_NONE
#include <string>
#include <GLFW/glfw3.h>
#include <gdt/math/vec.h>
using namespace gdt;

struct GLFWindow {
    GLFWindow(const std::string &title);
    ~GLFWindow();

    /*! put pixels on the screen ... */
    virtual void draw()
    { /* empty - to be subclassed by user */ }

    /*! callback that window got resized */
    virtual void resize(const vec2i &newSize)
    { /* empty - to be subclassed by user */ }

    virtual void key(int key, int mods)
    {}

    /*! callback that window got resized */
    virtual void mouseMotion(const vec2i &newPos)
    {}

    /*! callback that window got resized */
    virtual void mouseButton(int button, int action, int mods)
    {}

    inline vec2i getMousePos() const
    {
        double x,y;
        glfwGetCursorPos(handle,&x,&y);
        return vec2i((int)x, (int)y);
    }

    /*! re-render the frame - typically part of draw(), but we keep
      this a separate function so render() can focus on optix
      rendering, and now have to deal with opengl pixel copies
      etc */
    virtual void render()
    { /* empty - to be subclassed by user */ }

    /*! opens the actual window, and runs the window's events to
      completion. This function will only return once the window
      gets closed */
    void run();

    /*! the glfw window handle */
    GLFWwindow *handle { nullptr };
};


#endif //WJ_PLAYGROUND_PTAPPWINDOW_H
