#ifndef POINT_H
#define POINT_H

#include "Shader.h"
#include <vector>

class Point {
public:
                    //shader, position, color, thickness
    Point(Shader& shader, const glm::vec3& position, const glm::vec3& color, float pointSize) : shader(shader), position(position), color(color), pointSize(pointSize) {
        initPointData();
        setupBuffers();
    }

    void Draw() {
        shader.Use();
        glPointSize(pointSize);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(pointVertices.size()));
        glBindVertexArray(0);
    }

private:
    Shader& shader;
    GLuint VAO, VBO;
    float pointSize;
    glm::vec3 position;
    glm::vec3 color;
    std::vector<GLfloat> pointVertices;

    void initPointData() {
        pointVertices.push_back(position.x);
        pointVertices.push_back(position.y);
        pointVertices.push_back(position.z);
    }

    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pointVertices.size(), pointVertices.data(), GL_STATIC_DRAW);

        // Assuming each vertex has 3 components (x, y, z)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif // !POINT_H
