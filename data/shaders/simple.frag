#version 330

#extension GL_ARB_explicit_attrib_location : require

in vec4 v_position;
in vec4 v_normal;
out vec4 fragColour;

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 specularPower;
    vec3 specularScale;
};

uniform material u_material;

void main() {
    /*fragColour = vec4(
        max(1, normalize(v_normal).y)*u_material.diffuse + u_material.ambient,
        1.0f
    );*/
    fragColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
