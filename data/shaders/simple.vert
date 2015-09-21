#version 140

#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 6) in vec2 i_tex;

uniform mat4 u_camera;
uniform mat4 model;

out vec4 v_position;
out vec4 v_normal;
out vec2 v_tex;

void main() {
    v_position = model * vec4(i_position, 1.0);
    gl_Position = u_camera * v_position;
    v_normal = normalize(model * vec4(i_normal, 0.0));
    v_tex = i_tex;
}
