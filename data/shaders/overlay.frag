#version 140

in vec4 v_normal;
in vec2 v_tex;
out vec4 fragColour;

uniform sampler2D baseStage, overlayStage;

void main() {
    vec4 base = texture(baseStage, v_tex);
    vec4 overlay = texture(overlayStage, v_tex);
    fragColour = mix(base, overlay, overlay.a);
    fragColour.a = 1.0f;
}
