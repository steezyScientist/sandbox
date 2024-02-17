#ifndef GRID_H
#define GRID_H

#include "Shader.h"
#include <vector>

class Grid {

public:
	Grid(Shader& shader) : shader(shader) {
		initGridData();
		setupBuffers();
	}

    void Draw() {
        shader.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(gridVertices.size()));
        glBindVertexArray(0);
    }
private:
    Shader& shader;
    GLuint VAO, VBO;
    std::vector<GLfloat> gridVertices;

    void initGridData() {
        for (float i = -5.0f; i <= 5.0f; i += 1.0f) {
            //X-Axis Lines
            gridVertices.push_back(i);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(-5.0f);

            gridVertices.push_back(i);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(5.0f);

            // Z-axis lines
            gridVertices.push_back(-5.0f);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(i);

            gridVertices.push_back(5.0f);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(i);
        }
    }
    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * gridVertices.size(), gridVertices.data(), GL_STATIC_DRAW);

        // Assuming each vertex has 3 components (x, y, z)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif // !GRID_H
