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

#include "Shader.h"
#include "Camera.hpp"
#include "Model.h"



//WINDOW SETTINGS
int windowWidth = 640;
int windowHeight = 480;
const char* windowTitle = "sandbox";
GLFWwindow* window = nullptr;

bool polyMode = false;

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

void MainLoop() {

    Shader shader("vertex.shader", "fragment.shader");;
    Model myModel("models\\Volleyball_ball.obj");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10.0f);

    while (!glfwWindowShouldClose(window))
    {

        Input();

        //////PRE-DRAW
        glViewport(0, 0, windowWidth, windowHeight);
        //background color
        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        ///////DRAW
        //use shader
        //draw transformations
        shader.Use();
        shader.setMat4("projection", projection);
        glm::mat4 view = gCamera.GetViewMatrix();
        shader.setMat4("view", view);
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        shader.setMat4("model", model);

        myModel.Draw(shader);

        //glUseProgram(0);

        ////////UPDATE SCREEN
        glfwPollEvents();
        glfwSwapBuffers(window);;
    }
}

void CleanUp() {
    glfwTerminate();
}


int main()
{
    
    Initialize();           //setip glfw window
    MainLoop();
    CleanUp();
    return 0;
}