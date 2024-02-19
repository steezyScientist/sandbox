#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <chrono>

#include "Shader.h"
#include "Camera.hpp"
#include "Model.h"
#include "Grid.h"
#include "Point.hpp"
#include "Line.h"



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
    //std::cout << "x:" << xpos << " y:" << ypos << std::endl;

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
void showFPS(GLFWwindow* window){
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime();

    elapsedSeconds = currentSeconds - previousSeconds;

    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
            << windowTitle << " "
            << "FPS: " << fps << " "
            << "Frame_Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());
        frameCount = 0;
    }
    frameCount++;
}

void Input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gCamera.MoveForward(speed);
        //printCamPosition();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gCamera.MoveBackward(speed);
        //printCamPosition();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gCamera.MoveLeft(speed);
        //printCamPosition();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gCamera.MoveRight(speed);
        //printCamPosition();
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        gCamera.MoveUp(speed);
        //printCamPosition();
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        gCamera.MoveDown(speed);
        //printCamPosition();
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

    Shader shader("lightvert.shader", "lightfrag.shader");
    //Grid grid(shader);
    Point myPoint(shader, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 1.0f), 10.f); //shader, position, color, thickness
    Line myLine(shader, glm::vec3(0.0f),glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0f, 0.0f, 1.0f), 0.05f);
    Line myLine2(shader, glm::vec3(0.0f),glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), 0.05f);
    Line myLine3(shader, glm::vec3(0.0f),glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), 0.05f); //y why
    Model myModel("models/nanosuit.obj");
    


    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10.0f);

    while (!glfwWindowShouldClose(window))
    {
        showFPS(window);
        Input();


        //////PRE-DRAW
        glViewport(0, 0, windowWidth, windowHeight);
        //background color
        glClearColor(0.91f, 0.76f, 0.65f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        //grid.Draw();

        //set the params.
        glm::mat4 model(1.0), view(1.0);
        view = gCamera.GetViewMatrix();

        ///////DRAW
        //use shader
        //draw transformations
        shader.Use();
        shader.setMat4("model", glm::mat4(1.0));
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", gCamera.getPosition());
        shader.setVec3("lightPos", glm::vec3(2.0f, 3.0f, 2.0f));
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::scale(glm::vec3(0.075f));
        shader.setMat4("model", model);

        //light.Use();
        //light.setMat4("model", glm::mat4(1.0));
        //light.setMat4("view", view);
        //light.setMat4("projection", projection);
        //light.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 0.0f));

        
        myModel.Draw(shader);
        myLine.Draw();
        myLine2.Draw();
        myLine3.Draw();
        //myPoint.Draw();


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


