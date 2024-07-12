#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"

Window::Window(GLFWwindow** win_ptr, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    window_ptr = win_ptr;
    init();
}

int Window::init() {
    //initialize glfw
    glewExperimental = true;
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    //create window
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    *window_ptr =glfwCreateWindow(windowWidth, windowHeight, "scene", NULL, NULL);
    if (!(*window_ptr)) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(*window_ptr);

    //initialize glew
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    return 0;
}
