#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<glm/glm.hpp>
#include "Collisions.hpp"
Simplex::Simplex(unsigned short maxSize) {
    vertices = new glm::vec3[maxSize];
    capacity = maxSize;
    size = 0;
    return;
}

float distanceToOrigin(glm::vec3 normal, glm::vec3 point) {
    return abs(glm::dot(normal, point)) / glm::length(normal);
}

glm::vec3 getPlaneNormal(glm::vec3 points[3]) {
    glm::vec3 normal = glm::cross((points[1]-points[0]), (points[2]-points[0]));
    normal = glm::dot(normal, (points[0]+points[1]+points[2])) > 0 ? -1.0f * normal : normal;
    return normal;
}
    
glm::vec3 Simplex::directionToOrigin() {
    glm::vec3 direction;
    if (size == 0) {
        direction = glm::vec3(1,0,0);
    } else if (size == 1) {
        direction = -1.0f * *(vertices + size - 1);
    } else if (size == 2) {
        glm::vec3 v1 = *(vertices + size - 1);
        glm::vec3 v2 = *(vertices + size - 2);
        direction = glm::cross((v1-v2),glm::cross(v1,v2));
        direction = glm::dot(direction, (v1+v2)) > 0 ? -1.0f * direction : direction;
    } else if (size == 3) {
        glm::vec3 v1 = *(vertices + size - 1);
        glm::vec3 v2 = *(vertices + size - 2);
        glm::vec3 v3 = *(vertices + size - 3);
        glm::vec3 points[] = {v1, v2, v3};
        direction = getPlaneNormal(points);
    } else {
        glm::vec3 points[3];
        for (int i = 0; i<3; i++) {
            points[i] = *(vertices + i + 0);
        }
        glm::vec3 normal = getPlaneNormal(points);
        float minDist = distanceToOrigin(normal, points[0]);
        direction = (minDist / glm::length(normal)) * normal;
        for (int i=0; i<=size-3; i++) {
            for (int j = 0; j<3; j++) {
                points[j] = *(vertices + j + 0);
            }
            normal = getPlaneNormal(points);
            float dist = distanceToOrigin(normal, points[0]);
            if (dist < minDist) {
                minDist = dist;
                direction = (dist / glm::length(normal)) * normal;
            }
        }
    }
    return direction;
}

bool Simplex::originEnclosed() {
    if (size <= 1) {
        return false;
    } else if (size == 2) {
        //check for covectors
        glm::vec3 v1 = vertices[0];
        glm::vec3 v2 = vertices[1];
        bool isCovector = abs((glm::dot(v1,v2)*glm::dot(v1,v2))-(glm::dot(v1,v1)*glm::dot(v2,v2))) < 0.0001f;
        if (isCovector) {
            printf("covector 2\n");
            return true;
        }
    } else if (size == 3) {
        for (int i=0; i<3; i++) {
            glm::vec3 v1 = vertices[i];
            glm::vec3 v2 = vertices[(i+1)%3];
            bool isCovector = abs((glm::dot(v1,v2)*glm::dot(v1,v2))-(glm::dot(v1,v1)*glm::dot(v2,v2))) < 0.0001f;
            //check for covectors
            if (isCovector) {
                printf("covector 3\n");
                return true;
            }
            glm::vec3 lastVector = vertices[(i+2)%3];
            glm::vec3 normal = glm::cross((v1-v2),glm::cross(v1,v2));
            normal = glm::dot(normal, (v1+v2)) > 0 ? -1.0f * normal : normal;
            //all of the third vectors must be in the same direction as the normal of the other two
            bool vectorOffSide = glm::dot(lastVector, normal) < 0;
            if (vectorOffSide) {
                return false;
            }
        }
    } else {
        /* should check for covectors and inner triangle vectors, not implemented */
        //check inside simplex
        for (int i=0; i<4; i++) {
            //create four triangles from last four vertices
            int indices[] = {(size-4)+(i+0)%4,(size-4)+(i+1)%4,(size-4)+(i+2)%4,(size-4)+(i+3)%4};
            glm::vec3 triangle[] = {vertices[indices[0]],vertices[indices[1]],vertices[indices[2]]};
            glm::vec3 lastVertex = vertices[indices[3]];
            glm::vec3 normal = getPlaneNormal(triangle);
            ///fourth vertex must be in the same direction as the normal
            if (glm::dot(lastVertex, normal) < 0) {
                return false;
            }
        }
    }
    printf("default\n");
    return true;
}

bool Simplex::containsVertex(glm::vec3 vertex) {
    for (int i=0; i<size; i++) {
        if (&vertices[i] == &vertex) {
            return true;
        }
    }
    return false;
}

bool Simplex::addVertex(glm::vec3 vertex) {
    if (size >= capacity) {
        return false;
    }
    *(vertices + size) = vertex;
    size++;
    return true;
}


Collider::Collider (std::vector<glm::vec3>* verts_ptr) {
    vertices_local = verts_ptr;
    vertices = std::vector<glm::vec3>((*vertices_local).size(),glm::vec3(0,0,0));
}

bool Collider::isColliding(Collider* other) {
    int maxCycles = vertices.size() * other->vertices.size();
    Simplex simplex((unsigned short) maxCycles);
    for (int i=0; i<maxCycles; i++) {
        glm::vec3 targetDirection = simplex.directionToOrigin();
        glm::vec3 minkowskiDifference =
            glm::vec3(transform * glm::vec4(minkowskiSupport(targetDirection),1.0f)) - 
            glm::vec3((other->transform) * glm::vec4(other->minkowskiSupport(-1.0f * targetDirection),1.0f));
        bool pointOnSide = glm::dot(targetDirection, minkowskiDifference) >= 0;
        if (!pointOnSide) {
            return false;
        } else if (simplex.originEnclosed()) {
            return true;
        } else {
            simplex.addVertex(minkowskiDifference);
        }
    }
    return false;
}

glm::vec3 Collider::minkowskiSupport(glm::vec3 direction) {
    glm::vec3 vertex = (*vertices_local)[0];
    glm::vec3 target = glm::normalize(direction);
    //find most extreme vertex in direction
    float maxDotProduct = 0;
    for (int i=0; i<vertices_local->size(); i++) {
        float dotProduct = glm::dot(glm::normalize((*vertices_local)[i]), target);
        if (dotProduct > maxDotProduct) {
            maxDotProduct = dotProduct;
            vertex = (*vertices_local)[i];
        }
    }
    return vertex;
}

void Collider::setTransform(glm::mat4 matrix) {
    transform = matrix;
    for (int i=0; i<vertices_local->size(); i++) {
        vertices[i] = glm::vec3(transform * glm::vec4((*vertices_local)[i],1.0f));
    }
}
