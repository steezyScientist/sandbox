#version 330 core

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 vertexColors;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 v_vertexColors;
    out vec2 TexCoord;

    uniform mat4 u_ModelMatrix;
    uniform mat4 u_PerspMatrix;

    void main()
    {
        v_vertexColors = vertexColors;
        TexCoord = aTexCoord;
        vec4 newPosition = u_PerspMatrix * u_ModelMatrix * vec4(position, 1.0f);
        gl_Position = newPosition;
    };
