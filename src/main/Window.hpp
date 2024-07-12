#pragma once
#ifndef WINDOW
#define WINDOW
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
    private:
        int init();
        GLFWwindow** window_ptr;
    public:
        GLFWwindow* window;
        int windowWidth;
        int windowHeight;
        Window(GLFWwindow** win_ptr, int width, int height);
};
#endif
