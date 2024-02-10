#version 330 core

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec2 aTexCoord;
    layout (location = 2) in vec3 aNormal;

    out vec2 TexCoord;
    out vec3 Normal;
    out vec3 FragPos;

    uniform mat4 u_ModelMatrix; //model
    uniform mat4 u_PerspMatrix; //view
    uniform mat4 u_View;        //projection
    

    void main()
    {
        FragPos = vec3(u_ModelMatrix * vec4(position, 1.0));
        Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal; 
        gl_Position = u_PerspMatrix * u_View * vec4(FragPos, 1.0);  
    }
