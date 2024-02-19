#ifndef LINE_H
#define LINE_H

#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>

class Line {
public:
    Line(Shader& shader, const glm::vec3& startPoint, const glm::vec3& endPoint, const glm::vec3& color, float thickness)
        : shader(shader), startPoint(startPoint), endPoint(endPoint), color(color), thickness(thickness) {
        initLineData();
        setupBuffers();
    }

    void Draw() {
        shader.Use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(lineVertices.size()));
        glBindVertexArray(0);
    }

private:
    Shader& shader;
    GLuint VAO, VBO;
    glm::vec3 startPoint, endPoint, color;
    float thickness;
    std::vector<GLfloat> lineVertices;

    void initLineData() {
        // Calculate orthogonal vector for thickness
        glm::vec3 orthogonal = glm::normalize(glm::cross(endPoint - startPoint, glm::vec3(0.0f, 1.0f, 0.0f))) * (thickness * 0.5f);

        // Vertices for the quad
        glm::vec3 quadVertices[4] = {
            startPoint - orthogonal,
            startPoint + orthogonal,
            endPoint + orthogonal,
            endPoint - orthogonal
        };

        // Triangles to fill the quad
        lineVertices.push_back(quadVertices[0].x);
        lineVertices.push_back(quadVertices[0].y);
        lineVertices.push_back(quadVertices[0].z);

        lineVertices.push_back(quadVertices[1].x);
        lineVertices.push_back(quadVertices[1].y);
        lineVertices.push_back(quadVertices[1].z);

        lineVertices.push_back(quadVertices[2].x);
        lineVertices.push_back(quadVertices[2].y);
        lineVertices.push_back(quadVertices[2].z);

        lineVertices.push_back(quadVertices[0].x);
        lineVertices.push_back(quadVertices[0].y);
        lineVertices.push_back(quadVertices[0].z);

        lineVertices.push_back(quadVertices[2].x);
        lineVertices.push_back(quadVertices[2].y);
        lineVertices.push_back(quadVertices[2].z);

        lineVertices.push_back(quadVertices[3].x);
        lineVertices.push_back(quadVertices[3].y);
        lineVertices.push_back(quadVertices[3].z);
    }

    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * lineVertices.size(), lineVertices.data(), GL_STATIC_DRAW);

        // Assuming each vertex has 3 components (x, y, z)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif // !LINE_H
