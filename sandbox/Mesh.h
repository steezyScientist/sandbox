#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include "Shader.h"

using namespace std;

struct Vertex {
	glm::vec3 Position; // Position
	glm::vec3 Normal;	// Normal
	glm::vec2 TexCoords;//Texture coordinates
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh {
public:
	//MESH DATA
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	//constructor
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) {
		//load data
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		//set vertex buffers and attributes
		this->setupMesh();
	}

	void Draw(Shader shader) {
		//bind textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;

		for (GLuint i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			stringstream ss;
			string number;
			string name = this->textures[i].type;

			if (name == "texture_diffuse") {
				ss << diffuseNr++;
			}
			else if (name == "texture_specular") {
				ss << specularNr++;
			}
			number = ss.str();
			glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		}

		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);

		//draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//set back to default vvalues for clean up
		for (GLuint i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	//render data
	GLuint VAO, VBO, EBO;

	void setupMesh() {
		//create buffers/array
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);
		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()*sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
	
		//vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		//vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

		//setup texture coord
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		//unbind vertex array
		glBindVertexArray(0);
	}
	
};






#endif // !MESH_H
