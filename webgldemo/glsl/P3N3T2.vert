uniform vec3 u_attnCoeffs; // omniLS attenuation coefficients

uniform mat4 u_model_view_proj_matrix; // model to view to projection space transform
uniform vec3 u_camera_pos; // camera pos in model space
uniform vec3 u_omniLS_pos; // omniLS pos in model space

attribute vec4 a_pos; // model space vertex position
attribute vec3 a_nrm; // model space vertex normal
attribute vec2 a_tex; // texture coordinates

varying vec3  v_n; // model space vertex normal
varying vec3  v_v; // model space vertex to camera vector
varying vec3  v_l; // model space vertex to omniLS vector
varying float v_a; // OmniLS attenuation
varying vec2  v_t; // texture coords

void main() {
    gl_Position = u_model_view_proj_matrix * a_pos;
    v_n = a_nrm;
    v_v = u_camera_pos - a_pos.xyz;
    v_l = u_omniLS_pos - a_pos.xyz;

    vec3 distances;
    distances.x = 1.0;
    distances.y = length(v_l);
    distances.z = distances.y * distances.y;
    v_a = 1.0 / dot(u_attnCoeffs, distances);

    v_t = a_tex;
}

