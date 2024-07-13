#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../main/Window.hpp"
#include "../main/ShaderLoader.hpp"
#include "../main/GJKCollisions.hpp"

class Polygon {
    private: 
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> colors;
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint colorBufferID;
    public:
        std::vector<glm::vec3> verticesWorld;
        glm::vec3 transform;
        glm::vec3 orientation;
        Polygon(std::vector<glm::vec3> vertices);
        void draw(GLuint programID);
        void setColor(glm::vec3 color);
};

void getInput(GLFWwindow* window, glm::vec3* direction);

const std::vector<glm::vec3> triangle_vertices ({
    glm::vec3(1.0f,0.0f,0.0f),
    glm::vec3(-0.5f,0.5f,0.0f),
    glm::vec3(-0.5f,-0.5f,0)});
const std::vector<glm::vec3> hexagon_vertices ({
    glm::vec3(0.0f,3.0f,0.0f),
    glm::vec3(-2.0f,2.0f,0.0f),
    glm::vec3(2.0f,2.0f,0),
    glm::vec3(-2.0f,-1.0f,0.0f),
    glm::vec3(2.0f,-1.0f,0.0f),
    glm::vec3(0.0f,-2.0f,0.0f)});

int main() {
    GLFWwindow* window;
    Window context(&window, 1020, 680);
    
    Polygon triangle(triangle_vertices);
    Polygon hexagon(hexagon_vertices);
    triangle.setColor(glm::vec3(0.4f, 0.9f, 0.3f));
    hexagon.setColor(glm::vec3(0.8f, 0.2f, 0.6f));
    hexagon.transform = glm::vec3(6,0,0);

    GLuint programID = LoadShaders((char*) "src/shaders/PolygonVertex.glsl", (char*) "src/shaders/PolygonFragment.glsl");

    // Set up camera matrix
    float zoom = 0.1f;
    glm::vec2 cameraTransform(0,0);
    float screenRatio = (float) context.windowHeight / context.windowWidth;
    GLuint cameraID = glGetUniformLocation(programID, "CAMERA");
    glm::mat4 camera;
    glm::vec3 targetDirection;
    double lastTime = glfwGetTime();
    do {
        //Controls
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;
        getInput(window, &targetDirection);
        const float speed = 3.0f;
        if (abs(targetDirection.x) > 0.001f || abs(targetDirection.y) > 0.001f) {
            triangle.orientation = glm::normalize(targetDirection);
            triangle.transform += speed * deltaTime * triangle.orientation;
        }
        if (isColliding(&triangle.verticesWorld, &hexagon.verticesWorld)) {
            hexagon.setColor(glm::vec3(0.2f, 0.6f, 0.7f));
        } else {
            hexagon.setColor(glm::vec3(0.8f, 0.2f, 0.6f));
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cameraTransform = (0.9f * cameraTransform) + glm::vec2(0.1f * triangle.transform);
        camera = glm::mat4(
            glm::vec4(screenRatio*zoom, 0, 0, -1 * cameraTransform.x*zoom*screenRatio),
            glm::vec4(0, zoom, 0, -1 * cameraTransform.y*zoom),
            glm::vec4(0,0,1,0), //glm is column major, needs transpose
            glm::vec4(0,0,0,1)); //this is for visual convineince
        glUniformMatrix4fv(cameraID, 1, GL_TRUE, &camera[0][0]);
        hexagon.draw(programID);
        triangle.draw(programID);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwWindowShouldClose(window) == 0);
    return 0;
}

Polygon::Polygon(std::vector<glm::vec3> verts) {
    transform = glm::vec3(0,0,0);
    orientation = glm::vec3(1,0,0);
    vertices = verts;
    colors = std::vector(vertices.size(), glm::vec3(1,1,1));
    verticesWorld = std::vector(vertices.size(), glm::vec3(0));

    //vertex array object
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    
    //vertex buffer object
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    //color buffer
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

}
void Polygon::draw(GLuint programID) {
    glm::mat4 translation(1.0f);
    translation = glm::translate(translation, transform);
    glm::vec4 dir = glm::vec4(glm::normalize(orientation),1);
    glm::mat4 rotation(glm::vec4(dir.x,dir.y,0,0),glm::vec4(dir.y, -1*dir.x,0,0),glm::vec4(0,0,1,0),glm::vec4(0,0,0,1));
    glm::mat4 model = translation*  rotation;
    GLuint modelID = glGetUniformLocation(programID, "MODEL");
    glUniformMatrix4fv(modelID, 1,GL_FALSE, &model[0][0]);
    //hitbox calculations
    for (int i=0; i<vertices.size(); i++) {
        verticesWorld[i] = glm::vec3(model * glm::vec4(vertices[i],1));
    }

    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);

    glUseProgram(programID);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Polygon:: setColor(glm::vec3 color) {
    glBindVertexArray(vertexArray);
    colors = std::vector(vertices.size(), color);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
}

void getInput(GLFWwindow* window, glm::vec3* direction) {
    bool inputState[4];
    inputState[0] = glfwGetKey(window, GLFW_KEY_RIGHT);
    inputState[1] = glfwGetKey(window, GLFW_KEY_UP);
    inputState[2] = glfwGetKey(window, GLFW_KEY_LEFT);
    inputState[3] = glfwGetKey(window, GLFW_KEY_DOWN);
    *direction = glm::vec3(inputState[0]-inputState[2], inputState[1]-inputState[3],0);
}
