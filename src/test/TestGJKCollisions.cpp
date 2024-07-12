#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include "../main/GJKCollisions.hpp"

int main();
bool approximatelyEqual(glm::vec3 v1, glm::vec3 v2, float tolerance);
bool testSupportFunction();
bool testSimplexNorm();
bool testContainsOrigin();
bool testGJKbasic();

int main() {
    if (testSupportFunction()) {
        printf("test support function successfull\n");
    }
    if (testSimplexNorm()) {
        printf("test simplex norm successful!!\n");
    }
    if (testContainsOrigin()) {
        printf("test contains origin successful!!\n");
    }
    if (testGJKbasic()) {
        printf("test gjk basic successfull!!\n");
    }
    return 0;
}

bool testSupportFunction() {
    std::vector<glm::vec3> verts({glm::vec3(0,1,0), glm::vec3(-1,-1,0), glm::vec3(1,-1,0)});
    glm::vec3 directions[] = {glm::vec3(1,2,0),glm::vec3(0.1f,-10,0), glm::vec3(-1,0,0)};
    glm::vec3 solutions[] = {glm::vec3(0,1,0),glm::vec3(1,-1,0), glm::vec3(-1,-1,0)};
    for (int i=0; i<3; i++) {
        glm::vec3* supportVector = GJKSupport(&verts, directions[i]);
        if (!approximatelyEqual(solutions[i], *supportVector, 0.0001f)) {
            printf("test support function failed :(\n");
            return false;
        }
    }
    return true;
}

bool testSimplexNorm() {
    bool success = true;
    Simplex s1;
    s1.addVertex(glm::vec3(1,-1,0));
    glm::vec3 n1 = s1.simplexNormal();
    if (!approximatelyEqual(glm::normalize(n1),glm::normalize(glm::vec3(-1,1,0)), 0.1f)) {
        printf("single point simplex did not find correct normal\n");
        success = false;
    }

    Simplex s2;
    s2.addVertex(glm::vec3(1,1,0));
    s2.addVertex(glm::vec3(1,-1,0));
    s2.addVertex(glm::vec3(3,0,0));
    glm::vec3 n2 = s2.simplexNormal();
    if (!approximatelyEqual(glm::normalize(n2),glm::normalize(glm::vec3(-1,0,0)), 0.1f)) {
        printf("triangle (2) simplex did not find correct normal\n");
        success = false;
    }

    Simplex s3;
    s3.addVertex(glm::vec3(99,-200,15));
    s3.addVertex(glm::vec3(0,-2,0));
    s3.addVertex(glm::vec3(-2,1,0));
    s3.addVertex(glm::vec3(-1,-2,0));
    glm::vec3 n3 = s3.simplexNormal();
    if (!approximatelyEqual(glm::normalize(n3),glm::normalize(glm::vec3(3,2,0)), 0.1f)) {
        printf("triangle (3) simplex did not find correct normal\n");
        success = false;
    }
    return success;
}

bool testContainsOrigin() {
    //false
    Simplex s1;
    s1.addVertex(glm::vec3(1,0,0));
    s1.addVertex(glm::vec3(0,1,0));
    s1.addVertex(glm::vec3(1,-1,0));
    bool s1Res = s1.containsOrigin() == false;
    if(s1Res) {
        printf("s1 success\n");
    } else {
        printf("s1 fail\n");
    }

    //true
    Simplex s2;
    s2.addVertex(glm::vec3(0,2,0));
    s2.addVertex(glm::vec3(-1,1,0));
    s2.addVertex(glm::vec3(0,-2,0));
    s2.addVertex(glm::vec3(1,1,0));
    bool s2Res = s2.containsOrigin() == true;
    if(s2Res) {
        printf("s2 success\n");
    } else {
        printf("s2 fail\n");
    }

    //true
    Simplex s3;
    s3.addVertex(glm::vec3(5,6,0));
    s3.addVertex(glm::vec3(4,-10,0));
    s3.addVertex(glm::vec3(3,1,0));
    s3.addVertex(glm::vec3(-1,1,0));
    s3.addVertex(glm::vec3(0,-1,0));
    bool s3Res = s3.containsOrigin() == true;
    if(s3Res) {
        printf("s3 success\n");
    } else {
        printf("s3 fail\n");
    }
    return s1Res && s2Res && s3Res;
}

bool testGJKbasic() {
    std::vector rhombus({glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(-1,0,0), glm::vec3(0,-1,0)});
    std::vector square1({glm::vec3(1,1,0), glm::vec3(-1,1,0), glm::vec3(-1,-1,0), glm::vec3(1,-1,0)});
    std::vector square2({glm::vec3(2,0,0), glm::vec3(3,0,0), glm::vec3(3,1,0), glm::vec3(2,1,0)});
    bool hit = isColliding(&rhombus, &square1);
    if (!hit) {
        printf("GJKBasic hit failed\n");
    }
    bool miss = isColliding(&rhombus, &square2);
    if (miss) {
        printf("GJKBasic miss failed\n");
    }
    return hit && (!miss);
}

bool approximatelyEqual(glm::vec3 v1, glm::vec3 v2, float tolerance) {
    return glm::length(v1 - v2) < tolerance;
}
