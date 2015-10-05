#version 140

in vec4 v_position;
in vec4 v_normal;
in vec2 v_tex;
out vec4 fragColour;

uniform sampler2D u_tex;

uniform vec3 gui_clip_start;
uniform vec3 gui_clip_end;

uniform vec3 u_textColour;

void main() {
    if(v_position.x > gui_clip_end.x || v_position.x < gui_clip_start.x
        || v_position.y > gui_clip_end.y || v_position.y < gui_clip_start.y) {

        discard;
    }

    float v = texture(u_tex, v_tex).r;
    fragColour.rgb = u_textColour * v;
    fragColour.a = v;
    if(fragColour.a <= 0.001) discard;
}
