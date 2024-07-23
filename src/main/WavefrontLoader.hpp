#pragma once
#ifndef WAVEFRONT_LOADER
#define WAVEFRONT_LOADER
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<cstring>
#include<string>
#include<glm/glm.hpp>
bool loadOBJ(
	const char* path,
	std::vector <glm::vec3> &out_vetecies,
	std::vector <glm::vec2> &out_uvs,
	std::vector <glm::vec3> &out_normals,
    std::vector <glm::vec3> &vertexList
);
#endif
