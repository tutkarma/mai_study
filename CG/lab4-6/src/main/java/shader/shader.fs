//Довженко А.А. М8О-307Б
//фрагментный шейдер, отвечающий за рассчет цвета

#version 120

uniform int axis;

uniform float red = 0;
uniform float green = 0;
uniform float blue = 0;

uniform vec3 light = vec3(0,0,1);
uniform vec3 normal;

uniform float ambientK = 1;
uniform float diffuseK = 1;
uniform float specularK = 1;

vec4 countColor(){
    vec3 col = vec3(red,green,blue);
    vec3 ambient = ambientK * col;

    vec3 n = normalize(normal);
    float diff = max(dot(n,light),0);
    vec3 diffuse = diffuseK * diff * col;

    float spec = pow(dot(n,light),32);
    vec3 specular = specularK * spec * vec3(1,1,1);

    return vec4(ambient + diffuse + specular,1);
}

void main() {
    if(axis == 1) {
        gl_FragColor = vec4(red,green,blue,1);
    } else {
        gl_FragColor = countColor();
    }
}