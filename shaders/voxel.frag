#version 460 core

in vec3 FragPos;
in vec3 Normal;
flat in uint MatrialID;

out vec4 FragColor;

vec3 getMaterialColor(uint id){
    switch(id){
        // Grass
        case 1u: return vec3(0.2, 0.8, 0.2);
        // Dirt
        case 2u: return vec3(0.5, 0.35, 0.25);
        // Grey
        case 3u: return vec3(0.5, 0.5,0.5);
        default: return vec3(1.0, 0.0, 1.0);
    }
}

void main(){
    vec3 N = normalize(Normal);
    // basic dir light setup here
    vec3 lightDir = normalize(vec3(0.4, 0.8, 0.5));
    float diff = max(dot(N, lightDir), 0.2);
    vec3 baseColor = getMaterialColor(MatrialID);
    FragColor = vec4(baseColor * diff, 1.0);
}

