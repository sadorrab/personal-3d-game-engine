/*
 * polygon collider
 * currently designed for 2d shapes in 3d space, but will generalize
 *
 * simplex
 *
 * GJK Algorithm
 */
#include <stdlib.h>
#include <math.h> //containsorigin
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GJKCollisions.hpp"

/* methods for GJKCollider */
GJKCollider::GJKCollider(std::vector<glm::vec3> vertices, glm::vec3 origin) {
    this->vertices = vertices;
    this->origin = origin;
}

glm::vec3* GJKCollider::GJKSupport(glm::vec3 direction) {
    glm::vec3* vertex = &this->vertices[0]; //assign default return value
    float maxDotProduct = 0;
    //search for most extreme vertex
    for (int i = 0; i<this->vertices.size(); i++) {
        float dotProduct = dot(glm::normalize(this->vertices[i] - origin), glm::normalize(direction));
        if (dotProduct >= maxDotProduct) {
            vertex = &this->vertices[i];
            maxDotProduct = dotProduct;
        }
    }
    return vertex;
}

/*Methods for Simplex*/
Simplex::Simplex() {
    size = 0;
    head = 0;
}

glm::vec3* lineSegmentNormal(glm::vec3 v1, glm::vec3 v2) {
    const glm::mat3 CLOCK_ROTATION(
        glm::vec3(0,1,0),
        glm::vec3(-1,0,0),
        glm::vec3(0,0,0));
    const glm::mat3 COUNTER_CLOCK_ROTATION(
        glm::vec3(0,-1,0),
        glm::vec3(1,0,0),
        glm::vec3(0,0,0));
    glm::vec3* normal = new glm::vec3();
    *normal = CLOCK_ROTATION * (v2 - v1);
    if (glm::dot(-1.0f*(v1+v2),*normal) < 0) {
        *normal = COUNTER_CLOCK_ROTATION * (v2 - v1);
    }
    return normal;
}

glm::vec3* Simplex::simplexNormal() {
    glm::vec3* normal = new glm::vec3();
    if (this->size == 0) {
        *normal = glm::vec3(1,0,0);
    } else if (this->size == 1) {
        *normal = -1.0f * this->vertices[0];
    } else if (this->size == 2) {
        normal = lineSegmentNormal(this->vertices[0], this->vertices[1]);
    } else {
        //determine closest side
        int maxDistIdx = 0;
        for (int i=0; i<3; i++) {
            if (glm::length(this->vertices[i]) > glm::length(this->vertices[maxDistIdx])) {
                maxDistIdx = i;
            }
        }
        normal = lineSegmentNormal(this->vertices[(maxDistIdx+1)%3], this->vertices[(maxDistIdx+2)%3]);
    }
    return normal;
}

void Simplex::addVertex(glm::vec3 vertex) {
    if (size < 3) {
        size++;
    }
    this->vertices[this->head] = vertex;
    this->head = (this->head +1)%3;
}

bool Simplex::containsOrigin() {
    if (size < 3) {
        return false;
    }
    //check if origin and 3rd point are on the same direction
    for (int i=0; i<3; i++) {
        glm::vec3* normal;
        normal = lineSegmentNormal(vertices[i], vertices[(i+1)%3]);
        if (glm::dot(*normal, vertices[(i+2)%3]) < 0) {
            return false;
        }
    }
    return true;
}

bool Simplex::hasVertex(glm::vec3 vertex) {
    const float TOLERANCE = 0.0001;
    for (int i=0; i<3; i++) {
        if (abs(1 - glm::dot(vertex, vertices[i])) < TOLERANCE) {
            return true;
        }
    }
    return false;
}

/*Miscelanious functions*/
bool isColliding(GJKCollider collider1, glm::vec3 collider1Position, GJKCollider collider2, glm::vec3 collider2Position) {
    Simplex simplex;
    CollisionStatus status = CollisionStatus::UNKNOWN;
    while (status == CollisionStatus::UNKNOWN) {
        glm::vec3* targetDirection = simplex.simplexNormal();
        glm::vec3 minkowskiDifference = (collider1Position + *collider1.GJKSupport(*targetDirection)) - (collider2Position + *collider2.GJKSupport(-1.0f * (*targetDirection)));
        bool minkowskiPointOnSide = glm::dot(minkowskiDifference, *targetDirection) >= 0;
        if ((!minkowskiPointOnSide) || simplex.hasVertex(minkowskiDifference)) {
            //miss when repeat vertex or new vertex is not on side of direction
            status = CollisionStatus::MISS;
        } else if (simplex.containsOrigin()) {
            //collision when minkowski difference contains origin
            status  = CollisionStatus::HIT;
        } else {
            simplex.addVertex(minkowskiDifference);
        }
    }
    return status == CollisionStatus::HIT;
}
