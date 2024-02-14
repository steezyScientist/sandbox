#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

class ShaderProgram
{
public:
	GLuint cShaderProgram = 0;

    void loadShader(const std::string& vSource, const std::string& fSource) {
        std::string vertexSource = LoadShaderString(vSource);
        std::string fragmentSource = LoadShaderString(fSource);

        this->cShaderProgram = CreateShaderProgram(vertexSource, fragmentSource);
    }

	void use() {
		glUseProgram(this->cShaderProgram);
	}

    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(cShaderProgram, name.c_str()), x, y, z);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(cShaderProgram, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(cShaderProgram, name.c_str()), x, y, z, w);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(cShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    std::string LoadShaderString(const std::string& filename) {
        std::string result = "";
        std::string line = "";
        std::ifstream myFile(filename.c_str());

        if (myFile.is_open()) {
            while (std::getline(myFile, line)) {
                result += line + '\n';
            }
            myFile.close();
        }
        return result;
    }

    GLuint CompileShader(GLuint type, const std::string& source) {
        GLuint shaderObject = 0;

        if (type == GL_VERTEX_SHADER) {
            shaderObject = glCreateShader(GL_VERTEX_SHADER);
        }
        else if (type == GL_FRAGMENT_SHADER) {
            shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        }


        const char* src = source.c_str();
        glShaderSource(shaderObject, 1, &src, nullptr);
        glCompileShader(shaderObject);
        //DEBUG//ERROR
        int success;
        glGetShaderiv(shaderObject, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            int length;
            glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
            char* errorMessages = new char[length];
            glGetShaderInfoLog(shaderObject, length, &length, errorMessages);
            if (type == GL_VERTEX_SHADER) {
                std::cout << "ERROR::VERTEX SHADER FAILED TO COMPILE\n" << errorMessages << std::endl;
            }
            else if (type == GL_FRAGMENT_SHADER) {
                std::cout << "ERROR::FRAGMENT SHADER FAILED TO COMPILE\n" << errorMessages << std::endl;
            }
            delete[] errorMessages;
            glDeleteShader(shaderObject);
            return 0;
        }

        return shaderObject;
    }

    GLuint CreateShaderProgram(const std::string& vertexshadersource, const std::string& fragmentshadersource)
    {
        GLuint programObject = glCreateProgram();

        GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
        GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

        glAttachShader(programObject, myVertexShader);
        glAttachShader(programObject, myFragmentShader);

        glLinkProgram(programObject);
        glValidateProgram(programObject);
        //ERROR HANDLING
        int  success;
        glGetProgramiv(programObject, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            int length;
            glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
            char* errorMessages = new char[length];
            glGetProgramInfoLog(programObject, length, &length, errorMessages);
            std::cout << "ERROR::PROGRAM FAILED TO LINK\n" << errorMessages << std::endl;
            delete[] errorMessages;
            glDeleteProgram(programObject);
            return 0;
        }

        glDetachShader(programObject, myVertexShader);
        glDetachShader(programObject, myFragmentShader);
        glDeleteShader(myVertexShader);
        glDeleteShader(myFragmentShader);


        return programObject;
    }

};