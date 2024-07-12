#pragma once
#ifndef SHADER_LOADER
#define SHADER_LOADER
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<sstream>
#include<fstream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
#endif
