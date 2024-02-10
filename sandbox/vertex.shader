#version 330 core

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    uniform mat4 u_ModelMatrix;
    uniform mat4 u_PerspMatrix;
    uniform mat4 u_View;
    

    void main()
    {
        vec4 newPosition = u_PerspMatrix * u_View *  u_ModelMatrix * vec4(position, 1.f);
        gl_Position = newPosition;
    };
