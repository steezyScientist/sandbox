#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "vertex.h"
#include "fragment.h"

//Window Settings
int windowWidth = 640;
int windowHeight = 480;
const char* windowTitle = "sandbox";
GLFWwindow* window = nullptr;

// Create Vertex Array Object
// Create Vertex Buffer Object
GLuint vao;
GLuint vbo;
GLuint GraphicsPipelineProgram;
GLuint shaderObject;
GLuint CompileShader(GLuint type, const std::string& source) {
      
    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
        std::cout << "VERTEX shader compiled" << std::endl;
    }
    else if (type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        std::cout << "FRAGMENT shader compiled" << std::endl;
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);
    return shaderObject;
}
GLuint CreateShaderProgram(const std::string& vertexshadersource,
    const std::string& fragmentshadersource)
{
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);
    glValidateProgram(programObject);
    return programObject;
}

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER)<< std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION)<< std::endl;
    std::cout << "Shading Language: "<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void Initialize() {
    glfwInit();
    if (!glfwInit())
    {
        std::cout << "Failed to initialize" << std::endl;
    }
    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

   window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
   if (!window)
   {
       std::cout << "Failed to create window" << std::endl;
   }
   glfwMakeContextCurrent(window);
 
   glewExperimental = GL_TRUE;
   glewInit();


   GetOpenGLVersionInfo();

}

void VertexSpecification() {
    const std::vector<GLfloat> vertexPosition{
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f 
    };
    //generate VAO
    //bind to VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //generate VBO
    //bind to VBO
    //populate the buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                vertexPosition.size() * sizeof(GLfloat),
                vertexPosition.data(),
                GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

void CreateGraphicsPipeline() {
    GraphicsPipelineProgram = CreateShaderProgram(vertexSource, fragmentSource);
}

void PreDraw(){

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(GraphicsPipelineProgram);

}

void Draw() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainLoop() {
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        PreDraw();
        Draw();

        //update
        glfwPollEvents();
        glfwSwapBuffers(window);;
    }
}

void CleanUp() {
    glfwTerminate();
}


int main()
{
    Initialize();
    VertexSpecification();
    CreateGraphicsPipeline();
    MainLoop();
    CleanUp();
    return 0;
}