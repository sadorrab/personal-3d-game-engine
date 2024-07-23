#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../main/Window.hpp"
#include "../main/Collisions.hpp"
#include "../main/ShaderLoader.hpp"
#include "../main/WavefrontLoader.hpp"

class Polytope {
    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> triangle_vertices;
        std::vector<glm::vec2> triangle_uvs;
        std::vector<glm::vec3> triangle_normals;
        std::vector<glm::vec4> triangle_colors;
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint normalBuffer;
        GLuint colorBuffer;
    public:
        Collider* hitbox;
        glm::vec3 acceleration;
        glm::vec3 velocity;
        glm::vec3 transform;
        Polytope(const char* modelPath);
        void draw(GLuint shaderProgram);
        void setColor(glm::vec4 color);
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* usrParam);

const std::vector<glm::vec3> verts({glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)});

int main() {
    GLFWwindow* window;
    Window context(&window, 1080,800);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    Polytope cylinder("models/cylinder.obj");
    Polytope sphere("models/sphere.obj");
    cylinder.transform = glm::vec3(0,8,0);
    cylinder.acceleration = glm::vec3(0.0f);
    cylinder.velocity= glm::vec3(0.0f);
    const glm::vec4 white(1.0f,1.0f,1.0f,1.0f);
    const glm::vec4 red(1.0f,0.0f,0.0f,1.0f);
    cylinder.setColor(white);
    sphere.setColor(white);

    GLuint shaderProgram = LoadShaders("src/shaders/SolidVertex.glsl", "src/shaders/SolidFragment.glsl");

    double lastTime = glfwGetTime();
    do {
        double currentTime = glfwGetTime();
        float deltatime = float(currentTime - lastTime);
        lastTime = currentTime;

        //input
        bool upPressed = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
        bool downPressed = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
        bool leftPressed = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
        bool rightPressed = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
        //collisions
        if (cylinder.hitbox->isColliding(sphere.hitbox)) {
            cylinder.setColor(red);
        } else {
            cylinder.setColor(white);
        }

        //movement
        glm::vec3 friction = -5.6f * cylinder.velocity;
        cylinder.acceleration = 
            float(upPressed - downPressed) * glm::vec3(0,1,0) +
            float(rightPressed - leftPressed) * glm::vec3(1,0,0) +
            friction;
        if (glm::dot(cylinder.velocity, cylinder.velocity) < 1) {
            cylinder.velocity += deltatime * cylinder.acceleration;
        }
        cylinder.transform += cylinder.velocity;
        
        glUseProgram(shaderProgram);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3(1,8,20), glm::vec3(0,4,0), glm::vec3(0,1,0));
        GLuint cameraLocation = glGetUniformLocation(shaderProgram, "VIEW");
        glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, &view[0][0]);
        //projection
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 1080 / (float) 800, 0.1f, 100.0f);
        GLuint projectionLocation = glGetUniformLocation(shaderProgram, "PROJECTION");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

        cylinder.draw(shaderProgram);
        sphere.draw(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwWindowShouldClose(window) == 0);
    return 0;
}

Polytope::Polytope(const char* modelPath) {
    bool loadModel = loadOBJ(modelPath, triangle_vertices, triangle_uvs, triangle_normals, vertices);
    if (!loadModel) {
        fprintf(stderr, "unable to load model: %s\n", modelPath);
    }
    
    //VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    //VBO
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size() * sizeof(glm::vec3), &triangle_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, triangle_normals.size() * sizeof(glm::vec3), &triangle_normals[0], GL_STATIC_DRAW);

    //color
    triangle_colors = std::vector<glm::vec4>(triangle_vertices.size(), glm::vec4(1.0f,1.0f,1.0f,1.0f));
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, triangle_colors.size() * sizeof(glm::vec4), &triangle_colors[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    transform = glm::vec3(0,0,0);
    //collider
    hitbox = new Collider(&vertices);
}
void Polytope::draw(GLuint shaderProgram) {
    //model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform);
    GLuint modelLocation = glGetUniformLocation(shaderProgram, "MODEL");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(2, 4, GL_FLOAT,GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, triangle_vertices.size());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);

    hitbox->setTransform(model);
}

void Polytope::setColor(glm::vec4 color) {
    triangle_colors = std::vector<glm::vec4>(triangle_vertices.size(), color);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, triangle_colors.size() * sizeof(glm::vec4), &triangle_colors[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* usrParam) {
    fprintf(stderr, "GL Callback: %s type = 0x%x, severity = 0x%x, message = %s\n", (type==GL_DEBUG_TYPE_ERROR ? "**GL Error**" : ""), type, severity, message);
}
