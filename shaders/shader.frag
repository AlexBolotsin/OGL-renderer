#version 330

out vec4 color;

in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;

struct DirectionalLight
{
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight dirLight;

void main()
{
    vec4 ambientColor = vec4(dirLight.color, 1.0) * dirLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(dirLight.direction)) * dirLight.diffuseIntensity, 0.0);
    vec4 diffuseColor = vec4(dirLight.color, 1.0f) * dirLight.diffuseIntensity * diffuseFactor;

    color = texture(theTexture, TexCoord) * (ambientColor + diffuseColor);
}