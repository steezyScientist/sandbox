
const char* fragmentSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 v_vertexColors;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, TexCoord);\n"
    "}\n\0";