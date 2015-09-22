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

void main() {
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
