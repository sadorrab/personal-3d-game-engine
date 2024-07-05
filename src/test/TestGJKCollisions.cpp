#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include "../main/GJKCollisions.hpp"

int main();

int main() {

    GJKCollider triangle(std::vector(3,glm::vec3(1,2,3)), glm::vec3(4,5,6));
    glm::vec3* supportVector = triangle.GJKSupport(glm::vec3(1,0,0));
    printf("(%f, %f, %f)\n", supportVector->x, supportVector->y, supportVector->z);
    return 0;
}
