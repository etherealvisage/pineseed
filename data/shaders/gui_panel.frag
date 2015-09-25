#version 140

#extension GL_ARB_explicit_attrib_location : require

const float pi = 3.14159265;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_tex;
out vec4 fragColour;

uniform int time;
uniform float gui_xdpcm, gui_ydpcm;
uniform float panel_activation;

uniform vec3 gui_clip_start;
uniform vec3 gui_clip_end;

void main() {
    if(v_position.x > gui_clip_end.x || v_position.x < gui_clip_start.x
        || v_position.y > gui_clip_end.y || v_position.y < gui_clip_start.y) {

        discard;
    }

    const float border_width = 0.3f;
    float xp = v_tex.x * gui_xdpcm;
    float yp = v_tex.y * gui_ydpcm;
    float xdist = min(xp, gui_xdpcm-xp);
    float ydist = min(yp, gui_ydpcm-yp);
    float md = min(xdist, ydist);
    if(md < border_width) {
        fragColour = mix(
            mix(vec4(0.0f, 0.0f, 0.0f, 0.5f),
                vec4(0.0f, 0.3f, 0.3f, 0.5f),
                panel_activation),
            vec4(0.3f, 0.3f, 0.0f, 1.0f),
            pow(md/border_width, 0.3f));
    }
    else {
        fragColour = vec4(0.3f, 0.3f, 0.0f, 1.0f);
    }
}
