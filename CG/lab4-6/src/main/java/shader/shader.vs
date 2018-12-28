//Довженко А.А. М8О-307Б
//вершинный шейдер, отвечающий за расположение точек

#version 120

attribute vec3 vertex;
uniform vec3 normal;

uniform int axis;

uniform mat4 matrix;

void main() {
    gl_Position = matrix * vec4(vertex, 1);
}