#ifndef SHADER_H
#define SHADER_H


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//GLuint cShaderProgram;

    //void loadShader(const std::string& vSource, const std::string& fSource) {
    //    std::string vertexSource = LoadShaderString(vSource);
    //    std::string fragmentSource = LoadShaderString(fSource);

    //    this->cShaderProgram = CreateShaderProgram(vertexSource, fragmentSource);
    //}
    GLuint Program;
    // Constructor generates the shader on the fly
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        try
        {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar* fShaderCode = fragmentCode.c_str();
        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Shader Program
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        // Print linking errors if any
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }
    void Use()
    {
        glUseProgram(this->Program);
    }
	//void use() {
	//	glUseProgram(this->cShaderProgram);
	//}

    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(Program, name.c_str()), x, y, z);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(Program, name.c_str()), x, y, z, w);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    //std::string LoadShaderString(const std::string& filename) {
    //    std::string result = "";
    //    std::string line = "";
    //    std::ifstream myFile(filename.c_str());

    //    if (myFile.is_open()) {
    //        while (std::getline(myFile, line)) {
    //            result += line + '\n';
    //        }
    //        myFile.close();
    //    }
    //    return result;
    //}

    //GLuint CompileShader(GLuint type, const std::string& source) {
    //    GLuint shaderObject = 0;

    //    if (type == GL_VERTEX_SHADER) {
    //        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    //    }
    //    else if (type == GL_FRAGMENT_SHADER) {
    //        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    //    }


    //    const char* src = source.c_str();
    //    glShaderSource(shaderObject, 1, &src, nullptr);
    //    glCompileShader(shaderObject);
    //    //DEBUG//ERROR
    //    int success;
    //    glGetShaderiv(shaderObject, GL_LINK_STATUS, &success);
    //    if (success == GL_FALSE) {
    //        int length;
    //        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
    //        char* errorMessages = new char[length];
    //        glGetShaderInfoLog(shaderObject, length, &length, errorMessages);
    //        if (type == GL_VERTEX_SHADER) {
    //            std::cout << "ERROR::VERTEX SHADER FAILED TO COMPILE\n" << errorMessages << std::endl;
    //        }
    //        else if (type == GL_FRAGMENT_SHADER) {
    //            std::cout << "ERROR::FRAGMENT SHADER FAILED TO COMPILE\n" << errorMessages << std::endl;
    //        }
    //        delete[] errorMessages;
    //        glDeleteShader(shaderObject);
    //        return 0;
    //    }

    //    return shaderObject;
    //}

    //GLuint CreateShaderProgram(const std::string& vertexshadersource, const std::string& fragmentshadersource)
    //{
    //    GLuint programObject = glCreateProgram();

    //    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
    //    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

    //    glAttachShader(programObject, myVertexShader);
    //    glAttachShader(programObject, myFragmentShader);

    //    glLinkProgram(programObject);
    //    glValidateProgram(programObject);
    //    //ERROR HANDLING
    //    int  success;
    //    glGetProgramiv(programObject, GL_LINK_STATUS, &success);
    //    if (success == GL_FALSE) {
    //        int length;
    //        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
    //        char* errorMessages = new char[length];
    //        glGetProgramInfoLog(programObject, length, &length, errorMessages);
    //        std::cout << "ERROR::PROGRAM FAILED TO LINK\n" << errorMessages << std::endl;
    //        delete[] errorMessages;
    //        glDeleteProgram(programObject);
    //        return 0;
    //    }

    //    glDetachShader(programObject, myVertexShader);
    //    glDetachShader(programObject, myFragmentShader);
    //    glDeleteShader(myVertexShader);
    //    glDeleteShader(myFragmentShader);


    //    return programObject;
    //}

};

#endif