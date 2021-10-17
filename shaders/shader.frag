#version 330


in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight
{
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight dirLight;
uniform Material material;

uniform vec3 eyePosition;

void main()
{
    vec4 ambientColor = vec4(dirLight.color, 1.0) * dirLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(dirLight.direction)), 0.0);
    vec4 diffuseColor = vec4(dirLight.color, 1.0) * dirLight.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);

    if (diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(dirLight.direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);

        if (specularFactor > 0.0)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(dirLight.color * material.specularIntensity * specularFactor, 1.0);
        }
    }

    color = texture(theTexture, TexCoord) * (ambientColor + diffuseColor + specularColor);
}