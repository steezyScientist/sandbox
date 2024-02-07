#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

// image loading library by Sean Barrett
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shaders.h"


//WINDOW SETTINGS
int windowWidth = 640;
int windowHeight = 480;
const char* windowTitle = "sandbox";
GLFWwindow* window = nullptr;
 
float gOffset;
float gRotate;

//PROCESS INPUTS
void Input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gOffset += 0.001;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gOffset -= 0.001;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gRotate -= 0.5;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gRotate += 0.5;
    }
}

// Create Vertex Array Object
// Create Vertex Buffer Object
GLuint vao = 0;
GLuint vbo = 0;
//Create index buffer object
GLuint ibo = 0;
GLuint texture;

ShaderProgram sandbox;

void HandleShader() {
    sandbox.setVertexSource("vertex.shader");
    sandbox.setFragSource("fragment.shader");
    sandbox.runShaderProgram();
}

//TEXTURE
void LoadCreateTexture() {
    
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
    unsigned char* data = stbi_load("texture_01_0.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
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
    const std::vector<GLfloat> vertexData{
        // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left
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
                vertexData.size() * sizeof(GLfloat),
                vertexData.data(),
                GL_STATIC_DRAW);


    const std::vector<GLuint> indexBufferData{ 0, 1, 3, 1, 2, 3 };
    //Index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                indexBufferData.size() * sizeof(GLuint),
                indexBufferData.data(),
                GL_STATIC_DRAW);

    //vertex point information
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*8, (void*)0);
    //color point information
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)(sizeof(GL_FLOAT) * 3));
    //texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)(sizeof(GL_FLOAT) * 6));


    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}


void PreDraw(){

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, windowWidth, windowHeight);
    //background color
    glClearColor(0.74902f, 0.847059f, 0.847059f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    //glUseProgram(GraphicsPipelineProgram);
    sandbox.use();



    //model transformation
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    model = glm::rotate(model, glm::radians(gRotate), glm::vec3(0.0f, 1.0f, 0.0f));

    GLuint modelMatrixLocation = glGetUniformLocation(sandbox.cShaderProgram, "u_ModelMatrix");
    if (modelMatrixLocation >= 0) {
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }
    else {
        std::cout << "Could not find u_ModelMatrix\n";
        exit(EXIT_FAILURE);
    }
    //projection matrix
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10.0f);
    GLuint PerspMatrixLocation = glGetUniformLocation(sandbox.cShaderProgram, "u_PerspMatrix");
    if (PerspMatrixLocation >= 0) {
        glUniformMatrix4fv(PerspMatrixLocation, 1, GL_FALSE, &perspective[0][0]);
    }
    else {
        std::cout << "Could not find u_PerspMatrix\n";
        exit(EXIT_FAILURE);
    }
}

void Draw() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
   
    //WIREFRAME MODE
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void MainLoop() {
    while (!glfwWindowShouldClose(window))
    {
        
        Input();
        PreDraw();
        Draw();

        //update screen
        glfwPollEvents();
        glfwSwapBuffers(window);;
    }
}

void CleanUp() {
    glfwTerminate();
}


int main()
{
    //setip glfw window
    Initialize();
    //setup geometry
    VertexSpecification();
    //load textures
    LoadCreateTexture();
    //load shaders
    HandleShader();
    MainLoop();
    CleanUp();
    return 0;
}