/*
 * polygon collider
 * currently designed for 2d shapes in 3d space, but will generalize
 *
 * simplex
 *
 * GJK Algorithm
 */
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>
#include "GJKCollisions.hpp"

GJKCollider::GJKCollider(std::vector<glm::vec3> vertices, glm::vec3 origin) {
    this->vertices = vertices;
    this->origin = origin;
}

glm::vec3* GJKCollider::GJKSupport(glm::vec3 direction) {
    glm::vec3* vertex = new glm::vec3(0,0,0);
    return vertex;
}
