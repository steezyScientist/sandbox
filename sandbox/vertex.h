const char* vertexSource = R"glsl(
    #version 410 core
    in vec4 position

    void main()
    {
        position = vec4(position.x, position.y, position.z, position.w);
    }
)glsl";