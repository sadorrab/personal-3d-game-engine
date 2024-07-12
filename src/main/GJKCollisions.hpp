#ifndef GJK_COLLISIONS
#define GJK_COLLISIONS
#pragma once
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Simplex {
    private:
        glm::vec3 vertices[3];
        short size;
        short head;
    public:
        /* constructor */
        Simplex();
        /* returns the vector in the direction which is most likely to enclose the origin */
        glm::vec3 simplexNormal();
        void addVertex(glm::vec3 vertex);
        bool containsOrigin();
        bool hasVertex(glm::vec3 vertex);
};

enum CollisionStatus {HIT, MISS, UNKNOWN};

glm::vec3* GJKSupport(std::vector<glm::vec3>* vertices, glm::vec3 direction);

bool isColliding(std::vector<glm::vec3>* collider1, std::vector<glm::vec3>* collider2);
#endif
