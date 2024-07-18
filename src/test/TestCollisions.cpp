#include <stdio.h>
#include <stdlib.h>
#include "../main/Collisions.hpp"

bool testDirectionToOrigin();
bool testOriginEnclosed();
bool testColliding();
bool testSupportFunction();

int main() {
    int success =
        testDirectionToOrigin() +
        testOriginEnclosed() +
        testSupportFunction() +
        testColliding();
    if (success >= 4) {
        printf("All tests passed!!!\n");
    }
    return 0;
}

bool vectorEquals(glm::vec3 v1, glm::vec3 v2, float tolerance) {
    glm::vec3 d = v2 - v1;
    return (d.x < tolerance) && (d.y < tolerance) && (d.z < tolerance);
}

bool testDirectionToOrigin() {
    bool success = true;
    Simplex s1(4);
    s1.addVertex(glm::vec3(-2,1,0));
    glm::vec3 expected1 = glm::normalize(glm::vec3(2,-1,0));
    glm::vec3 direction1 = glm::normalize(s1.directionToOrigin());
    if (!vectorEquals(expected1, direction1, 0.0001f)) {
        printf("Direction to Origin in simplex 1: expected (%f, %f, %f) but direction was (%f, %f, %f)\n", expected1.x,expected1.y,expected1.z,direction1.x,direction1.y,direction1.z);
        success = false;
    }

    Simplex s2(4);
    s2.addVertex(glm::vec3(-1,1,0));
    s2.addVertex(glm::vec3(0,-1,0));
    glm::vec3 expected2 = glm::normalize(glm::vec3(2,1,0));
    glm::vec3 direction2 = glm::normalize(s2.directionToOrigin());
    if (!vectorEquals(expected2, direction2, 0.0001f)) {
        printf("Direction to Origin in simplex 2: expected (%f, %f, %f) but direction was (%f, %f, %f)\n", expected2.x,expected2.y,expected2.z,direction2.x,direction2.y,direction2.z);
        success = false;
    }

    Simplex s3(4);
    s3.addVertex(glm::vec3(9,1,-8)); //unused vertex
    s3.addVertex(glm::vec3(-2,4,3));
    s3.addVertex(glm::vec3(1,3,-2));
    s3.addVertex(glm::vec3(-5,-2,1));
    glm::vec3 expected3 = glm::normalize(glm::vec3(28,-21,21));
    glm::vec3 direction3 = glm::normalize(s3.directionToOrigin());
    if (!vectorEquals(expected3, direction3, 0.0001f)) {
        printf("Direction to Origin in simplex 3: expected (%f, %f, %f) but direction was (%f, %f, %f)\n", expected3.x,expected3.y,expected3.z,direction3.x,direction3.y,direction3.z);
        success = false;
    }
    return success;
}

bool testOriginEnclosed() {
    bool success = true;
    Simplex s1(8);
    s1.addVertex(glm::vec3(0, -2, 4));
    s1.addVertex(glm::vec3(3, 5, 2));
    s1.addVertex(glm::vec3(2, -3, -1));
    s1.addVertex(glm::vec3(-4, 2, 0));
    bool expected1 = true;
    if (expected1 != s1.originEnclosed()) {
        printf("Origin Enclosed in simplex 1: origin should be enclosed but is not\n");
        success = false;
    }

    Simplex s2(8);
    s2.addVertex(glm::vec3(3, -2, 4));
    s2.addVertex(glm::vec3(3, 5, 2));
    s2.addVertex(glm::vec3(2, -2, 2));
    s2.addVertex(glm::vec3(-4, 2, 0));
    bool expected2 = false;
    if (expected2 != s2.originEnclosed()) {
        printf("Origin Enclosed in simplex 2: origin is falsely enclosed\n");
        success = false;
    }

    Simplex s3(4);
    s3.addVertex(glm::vec3(-1, -1, 0));
    s3.addVertex(glm::vec3(1, -1, 0));
    s3.addVertex(glm::vec3(0, -2, 0));
    bool expected3 = false;
    if (expected3 != s3.originEnclosed()) {
        printf("Oringin Enclosed in simplex 3: should not have enough points to enclose origin\n");
        success = false;
    }
    return success;
}

bool testColliding() {
    bool success = true;
    std::vector<glm::vec3> square1_vertices({
        glm::vec3(0,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,1,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1),
        glm::vec3(1,0,1),
        glm::vec3(1,1,1),
        glm::vec3(0,1,1)});
    std::vector<glm::vec3> square2_vertices({
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(-1,0,0),
        glm::vec3(0,-1,0),
        glm::vec3(1,0,1),
        glm::vec3(0,1,1),
        glm::vec3(-1,0,1),
        glm::vec3(0,-1,1)});
    Collider square1(&square1_vertices);
    Collider square2(&square2_vertices);
    //unit vectors
    const glm::vec4 I(1,0,0,0);
    const glm::vec4 J(0,1,0,0);
    const glm::vec4 K(0,0,1,0);
    /* test 1 overlapping in default position */
    glm::mat4 nonTransform(I,J,K,glm::vec4(0,0,0,1));
    square1.setTransform(nonTransform);
    square2.setTransform(nonTransform);
    bool expected1 = true;
    if (expected1 != square1.isColliding(&square2)) {
        printf("Colliding in test 1: should be colliding but is not\n");
        success = false;
    }

    /* test 2 overlapping with square1 displaced */
    glm::mat4 shift2(I,J,K,glm::vec4(-1,-1,0,1));
    square1.setTransform(shift2);
    square2.setTransform(nonTransform);
    bool expected2 = true;
    if (expected2 != square1.isColliding(&square2)) {
        printf("Colliding in test 2: should be colliding but is not\n");
        success = false;
    }

    /* test 3 nonoverlapping displaced on z */
    glm::mat4 shift3(I,J,K,glm::vec4(0,0,3,1));
    square1.setTransform(nonTransform);
    square2.setTransform(shift3);
    bool expected3 = false;
    if (expected3 != square1.isColliding(&square2)) {
        printf("Colliding in test 3: should not be colliding but is\n");
        success = false;
    }

    /* test 4 nonoverlapping generally displaced */
    glm::mat4 shift4(I,J,K,glm::vec4(-4,2,1,1));
    square1.setTransform(shift4);
    square2.setTransform(nonTransform);
    bool expected4 = false;
    if (expected4 != square1.isColliding(&square2)) {
        printf("Colliding in test 4: should not be colliding but is\n");
        success = false;
    }

    return success;
}

bool testSupportFunction() {
    bool success = true;
    std::vector<glm::vec3> verts({
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(-1,0,0),
        glm::vec3(0,-1,0),
        glm::vec3(1,0,1),
        glm::vec3(0,1,1),
        glm::vec3(-1,0,1),
        glm::vec3(0,-1,1)});
    Collider square(&verts);
    glm::mat4 transform(1.0f);
    square.setTransform(transform);
    /* test */
    glm::vec3 dir1(5,1,10);
    glm::vec3 expected1 = glm::normalize(glm::vec3(1,0,1));
    glm::vec3 res1 = glm::normalize(square.minkowskiSupport(dir1));
    if (!vectorEquals(expected1, res1, 0.0001f)) {
        printf("test support function 1: expected (%0.1f, %0.1f, %0.1f) but got (%0.1f, %0.1f, %0.1f)\n", expected1.x,expected1.y,expected1.z,res1.x,res1.y,res1.z);
        success = false;
    }
    return success;
}


