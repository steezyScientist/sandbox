#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    float ambientColor = 0.1f;
    vec3 ambient = ambientColor * lightColor;
    //diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float NDotL = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = lightColor * NDotL;

    //specular
    float specularColor = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularColor * spec * lightColor;
    //vec3 specular = vec3(0.0f);

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0); // set all 4 vector values to 1.0
}