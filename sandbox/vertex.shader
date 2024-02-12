#version 330 core

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 normal;

    uniform mat4 u_ModelMatrix;
    uniform mat4 u_PerspMatrix;
    uniform mat4 u_View;
    
    out vec3 Normal;
    out vec3 FragPos;

    void main()
    {
        Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
        FragPos = vec3(u_ModelMatrix * vec4(position, 1.0f));


        vec4 newPosition = u_PerspMatrix * u_View * vec4(position, 1.0);
        gl_Position = newPosition;
    }
