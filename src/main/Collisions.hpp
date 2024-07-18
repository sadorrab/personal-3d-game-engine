#ifndef COLLISIONS
#define COLLISIONS
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<glm/glm.hpp>
class Simplex {
    private:
        glm::vec3* vertices;
        unsigned short size;
        unsigned short capacity;
    public:
        Simplex(unsigned short maxSize);
        glm::vec3 directionToOrigin();
        glm::vec3* minkowskiSupport(glm::vec3 direction);
        bool originEnclosed();
        bool containsVertex(glm::vec3 vertex);
        bool addVertex(glm::vec3 vertex);
};
class Collider {
    private:
        glm::vec3 collisionNormal;
        glm::mat4 transform;
    public:
        std::vector<glm::vec3>* vertices_local;
        glm::vec3 centerOfMass;
        std::vector<glm::vec3> vertices;
        bool isColliding(Collider* other);
        Collider (std::vector<glm::vec3>* verts_ptr);
        void setTransform(glm::mat4 matrix);
        glm::vec3 minkowskiSupport(glm::vec3 direction);
};
#endif
