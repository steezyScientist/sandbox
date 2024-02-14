#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// image loading library by Sean Barrett
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "Mesh.h"

using namespace std;

GLint TextureFromFile(const char* path, string directory);

class Model {
public:
	//functions
	//constructor, filepath to 3D model
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;

	Model(const GLchar* path) {
		this->loadModel(path);
	}
	//draw the model
	void Draw(ShaderProgram shader) {
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			this->meshes[i].Draw(shader);
		}
	}

	void setPath(const GLchar* path) {
		this->loadModel(path);
	}

private:

	
	//load the model with assimp
	void loadModel(string path) {
		//read file via assimp
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		//check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is not zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		//retrieve the directory
		this->directory = path.substr(0, path.find_last_of('/'));
		//process assimps root node recursively
		this->processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene) {
		for (GLuint i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		for (GLuint i = 0; i < node->mNumChildren; i++) {
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		//data
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;

		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			glm::vec3 vector;

			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			// Texture Coordinates
			if (mesh->mTextureCoords[0]) //does the mesh contain texture coords
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}
		//process indices
		for (GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; j++){

				indices.push_back(face.mIndices[j]);
			
			}

		}
		//process material
		if (mesh->mMaterialIndex >= 0){

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// Diffuse maps
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(),
				diffuseMaps.end());
			// Specular maps
			vector<Texture> specularMaps = this->loadMaterialTextures(
				material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(),
				specularMaps.end());


		
		}

		return Mesh(vertices, indices, textures);
	}

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
		vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++) {
				if (textures_loaded[j].path == str) {
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip) {
				//if texture hasnt loaded, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture);
			}

		}
		return textures;
	}
};

GLint TextureFromFile(const char* path, string directory) {
	string filename = string(path);
	filename = directory + '/' + filename;

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAILED TO LOAD" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

#endif // !MODEL_H
