#ifndef GJK_COLLISIONS
#define GJK_COLLISIONS
#pragma once
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>

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
#endif
