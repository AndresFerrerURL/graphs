#version 330

in vec3 a_vertex;
in vec3 a_color;
in vec2 a_uv;
out vec2 v_uv;
in vec3 a_normal;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 v_color;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_vertex, 1.0);
    v_color = a_color;
    v_uv = a_uv;
    v_normal = a_normal;
}
