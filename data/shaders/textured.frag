#version 140

in vec4 v_normal;
in vec2 v_tex;
out vec4 fragColour;

uniform sampler2D u_tex;

void main() {
    fragColour = texture(u_tex, v_tex);
    if(fragColour.a <= 0.001) discard;
}
