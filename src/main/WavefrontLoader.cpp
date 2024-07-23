#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<cstring>
#include<string>
#include<glm/glm.hpp>
#include "WavefrontLoader.hpp"

bool loadOBJ(
	const char* path,
	std::vector <glm::vec3> &out_vetecies,
	std::vector <glm::vec2> &out_uvs,
	std::vector <glm::vec3> &out_normals,
    std::vector <glm::vec3> &vertexList
) {
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndicies;
	std::vector<glm::vec2> tmp_uvs;
	std::vector<glm::vec3> tmp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		fprintf(stderr, "cannot read obj file\n");
		return false;
	}

	do {
		char lineHead[128]; //first word of line
		int res = fscanf(file, "%s", lineHead);
		if (res == EOF) {
			break;
		}
		if (strcmp(lineHead, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertexList.push_back(vertex);
		} else if (strcmp(lineHead, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tmp_uvs.push_back(uv);
		} else if (strcmp(lineHead, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmp_normals.push_back(normal);
		} else if (strcmp(lineHead, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				fprintf(stderr, "parser cannot read file\n");
				return false;
			}
			for (int i=0; i<3; i++) {
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndicies.push_back(normalIndex[i]);
			}
		}
	} while(true);
	for (int i=0; i<vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = vertexList[vertexIndex - 1];
		out_vetecies.push_back(vertex);
	}
	for (int i=0; i<uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = tmp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	for (int i=0; i<normalIndicies.size(); i++) {
		unsigned int normalIndex = normalIndicies[i];
		glm::vec3 normal = tmp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return true;
}
