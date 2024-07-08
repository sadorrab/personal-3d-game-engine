#ifndef GJK_COLLISIONS
#define GJK_COLLISIONS
#pragma once
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GJKCollider {
    private:
        std::vector<glm::vec3> vertices;
        glm::vec3 origin;
    public:
        /* constructor 
         * makes shallow copies of parameters
         */
        GJKCollider(std::vector<glm::vec3> vertices, glm::vec3 origin);

        /* returns a pointer to the vertex in vertices whose vector with the shape origin has the greatest dot product with direction
         * this is the most extreme vertex in the direction of the parameter
         */
        glm::vec3* GJKSupport(glm::vec3 direction);
};

class Simplex {
    private:
        glm::vec3 vertices[3];
        short size;
        short head;
    public:
        /* constructor */
        Simplex();
        /* returns the vector in the direction which is most likely to enclose the origin */
        glm::vec3* simplexNormal();
        void addVertex(glm::vec3 vertex);
        bool containsOrigin();
        bool hasVertex(glm::vec3 vertex);
};

enum CollisionStatus {HIT, MISS, UNKNOWN};

bool isColliding(GJKCollider collider1, glm::vec3 collider1Position, GJKCollider collider2, glm::vec3 collider2Position);
#endif
