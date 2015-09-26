#version 140

#extension GL_ARB_explicit_attrib_location : require

const float pi = 3.14159265;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_tex;
out vec4 fragColour;

uniform int time;
uniform float gui_xdpi, gui_ydpi;
uniform float button_activation, button_click;

uniform vec3 gui_clip_start;
uniform vec3 gui_clip_end;

void main() {
    if(v_position.x > gui_clip_end.x || v_position.x < gui_clip_start.x
        || v_position.y > gui_clip_end.y || v_position.y < gui_clip_start.y) {

        discard;
    }

    const float border_width = 0.3f;
    float xd = (min(v_tex.x, 1.0f-v_tex.x) * gui_xdpi);
    float yd = (min(v_tex.y, 1.0f-v_tex.y) * gui_ydpi);

    float md = min(xd, yd);
    if(md < border_width) {
        vec4 activated = vec4(1.0f, 1.0f, 0.0f, 1.0f);
        vec4 deactivated = vec4(0.1f, 0.1f, 0.0f, 1.0f);
        vec4 border = mix(deactivated, activated, button_activation);
        fragColour = mix(vec4(0.4f, 0.4f, 0.0f, 1.0f), border,
            md/border_width);
    }
    else {
        vec4 activated = vec4(0.1f, 0.1f, 0.0f, 1.0f);
        vec4 deactivated = vec4(0.4f, 0.4f, 0.0f, 1.0f);
        fragColour = mix(deactivated, activated, button_activation);
        vec4 clicked = vec4(0.4f, 0.4f, 0.4f, 1.0f);
        fragColour = mix(fragColour, clicked, button_click/2.0f);
    }
}
