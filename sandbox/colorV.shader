#version 330 core

    layout (location = 0) in vec3 position;

    uniform mat4 u_ModelMatrix;
    uniform mat4 u_PerspMatrix;
    uniform mat4 u_View;
    

    void main()
    {
        vec4 newPosition = u_PerspMatrix * u_View *  u_ModelMatrix * vec4(position, 1.f);
        gl_Position = newPosition;
    }
