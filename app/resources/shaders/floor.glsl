//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D floor_texture;

uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform float cutOff;
uniform float outerCutOff;
uniform float intensity;

uniform vec3 dirLightDirection;
uniform vec3 dirLightColor;
uniform float dirLightIntensity;

void main() {
    vec3 normal = vec3(0.0, 1.0, 0.0);
    vec3 baseColor = texture(floor_texture, TexCoords).rgb;

    vec3 ambient = 0.6 * baseColor;

    //Spot light (saucer)
    vec3 toLight = normalize(lightPos - FragPos);
    float diff = max(dot(normal, toLight), 0.0);

    float theta = dot(normalize(-lightDir), toLight);
    float epsilon = cutOff - outerCutOff;
    float spot = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.02 * distance + 0.002 * distance * distance);

    vec3 spotDiffuse = diff * lightColor * intensity * spot * attenuation;

    // Directional light (moonlight)
    float dirDiff = max(dot(normal, normalize(-dirLightDirection)), 0.0);
    vec3 dirDiffuse = dirDiff * dirLightColor * dirLightIntensity;

    vec3 result = (ambient + spotDiffuse + dirDiffuse) * baseColor;
    FragColor = vec4(result, 1.0);
}