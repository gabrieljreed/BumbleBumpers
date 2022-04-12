#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 ColorOut;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main() {
    // Texture color
    vec3 color = texture(texture1, TexCoord).rgb;
    
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambientColor = lightColor * ambientStrength;

    // Diffuse
    vec3 norm = normalize(Normal);
    //vec3 lightDirection = normalize(lightPosition - FragPos);  // Point light-ish
    vec3 lightDirection = normalize(-lightPosition); // Directional light 
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = lightColor * diff;

    vec3 result = (ambientColor + diffuse) * color;
    //result = ambientStrength * color;

    //result = texture(texture1, TexCoord).rgb;
    ColorOut = vec4(result, 1.0);
}
