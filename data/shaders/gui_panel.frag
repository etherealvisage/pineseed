#version 140

#extension GL_ARB_explicit_attrib_location : require

const float pi = 3.14159265;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_tex;
out vec4 fragColour;

uniform int time;
uniform float gui_xdpcm, gui_ydpcm;
uniform float gui_trans;
uniform float panel_activation;

void main() {
    /*
    float xdist = (min(v_tex.x, 1.0f-v_tex.x) / gui_xscale) * panel_xscale;
    float ydist = (min(v_tex.y, 1.0f-v_tex.y) / gui_yscale) * panel_yscale;

    vec4 highlight = vec4(0.3f, 0.3f, 0.5f, 1.0f);
    float tensec_progress = (time % 5000) / 5000.0f;
    float highlight_mix = pow(0.1f, distance(v_position.x+1.3f, tensec_progress*2.6f)*8)/8.0f;

    if(xdist <= 5.0f || ydist <= 5.0f) {
        vec4 activated = vec4(1.0f, 1.0f, 0.0f, 1.0f);
        vec4 deactivated = vec4(0.1f, 0.1f, 0.0f, 1.0f);
        vec4 border = mix(deactivated, activated, panel_activation);
        fragColour = mix(vec4(0.3f, 0.3f, 0.0f, 1.0f), border,
            distance(5.0f, min(xdist,ydist)) / 5.0f);
    }
    else {
    }
    fragColour = mix(fragColour, highlight, highlight_mix);
    */

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
