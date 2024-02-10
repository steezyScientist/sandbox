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

#include "shaderClass.h"
#include "Camera.hpp"


//WINDOW SETTINGS
int windowWidth = 640;
int windowHeight = 480;
const char* windowTitle = "sandbox";
GLFWwindow* window = nullptr;

bool polyMode = false;

float gOffset;
float gRotate;


// Create Vertex Array Object
// Create Vertex Buffer Object
GLuint vao = 0;
GLuint lightVao = 0;
GLuint vbo = 0;
//Create index buffer object
GLuint ibo = 0;
GLuint texture;

//model GLuint
glm::mat4 model = glm::mat4(1.f);
glm::mat4 model2 = glm::mat4(1.f);
glm::mat4 view = glm::mat4(1.f);
glm::mat4 perspective = glm::mat4(1.f);

//lighting
glm::vec3 lightPos(.6f, 0.2f, -2.0f);

ShaderProgram sandbox;
ShaderProgram lightbox;
Camera gCamera;

float speed = 0.001;
float sens = 0.1;

//PROCESS INPUTS
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    std::cout << "x:" << xpos << " y:" << ypos << std::endl;

    float xPos = xpos * sens;
    float yPos = ypos * sens;

    gCamera.MouseLook(xPos, yPos);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {

        switch (polyMode) {
        case (true):
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            polyMode = false;
            break;
        case (false):
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            polyMode = true;
            break;
        }

    }
}

void Input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gCamera.MoveForard(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gCamera.MoveBackward(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gCamera.MoveLeft(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gCamera.MoveRight(speed);
    }

}


void HandleShader() {
    sandbox.setVertexSource("vertex.shader");
    sandbox.setFragSource("fragment.shader");
    sandbox.runShaderProgram();
    lightbox.setVertexSource("colorV.shader");
    lightbox.setFragSource("colorF.shader");
    lightbox.runShaderProgram();

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
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
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
        // positions      // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
    //vertex point information
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)0);    
    //normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)3);

    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)0);


    const std::vector<GLuint> indexBufferData{ 0, 1, 3, 1, 2, 3 };
    //Index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indexBufferData.size() * sizeof(GLuint),
        indexBufferData.data(),
        GL_STATIC_DRAW);


    //texture
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (GLvoid*)(sizeof(GL_FLOAT) * 3));




    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}


void PreDraw() {

    glViewport(0, 0, windowWidth, windowHeight);
    //background color
    glClearColor(0.74902f, 0.847059f, 0.847059f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // bind Texture
    //glBindTexture(GL_TEXTURE_2D, texture);
}


void Draw() {
    //glUseProgram(GraphicsPipelineProgram);

    sandbox.use();
    //CAMERA AND WORLD MATRIX
    sandbox.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    sandbox.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    sandbox.setVec3("lightPos", lightPos);
    sandbox.setVec3("viewPos", gCamera.getPosition());

    //model transformation
    model = glm::mat4(1.0f);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, -2.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    sandbox.setMat4("u_ModelMatrix", model);

    view = gCamera.GetViewMatrix();
    sandbox.setMat4("u_View", view);

    //projection matrix
    perspective = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10.0f);
    sandbox.setMat4("u_PerspMatrix", perspective);


    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  

    lightbox.use();
    lightbox.setVec3("lightPos", lightPos);
    lightbox.setMat4("u_PerspMatrix", perspective);
    lightbox.setMat4("u_View", view);
    model = glm::mat4(1.f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f));
    lightbox.setMat4("u_ModelMatrix", model);

    glBindVertexArray(lightVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);




    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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