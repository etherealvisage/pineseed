#version 140

in vec4 v_position;
in vec4 v_normal;
in vec2 v_tex;
out vec4 fragColour;

uniform sampler2D u_tex;

uniform vec3 gui_clip_start;
uniform vec3 gui_clip_end;

void main() {
    if(v_position.x > gui_clip_end.x || v_position.x < gui_clip_start.x
        || v_position.y > gui_clip_end.y || v_position.y < gui_clip_start.y) {

        discard;
    }

    fragColour = texture(u_tex, v_tex);
    if(fragColour.a <= 0.001) discard;
}
