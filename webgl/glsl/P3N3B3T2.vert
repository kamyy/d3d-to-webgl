uniform vec3 u_attnCoeffs; // omniLS attenuation coefficients

uniform mat4 u_model_view_proj_matrix; // model to view to projection space transform
uniform vec3 u_camera_pos; // camera pos in model space
uniform vec3 u_omniLS_pos; // omniLS pos in model space
uniform vec3 u_up_dir; // world up in model space

attribute vec4 a_pos; // model space vertex position
attribute vec3 a_nrm; // model space vertex normal
attribute vec3 a_bnm; // model space vertex binormal
attribute vec2 a_tex; // texture coords

varying vec3  v_v; // tangent space vertex to camera vector
varying vec3  v_l; // tangent space vertex to omniLS vector
varying vec3  v_u; // tangent space world up vector
varying float v_a; // OmniLS attenuation
varying vec2  v_t; // texture coords

void main() {
    gl_Position = u_model_view_proj_matrix * a_pos;

    // create model space to tangent space transform
    mat3 m = mat3(cross(a_bnm, a_nrm), a_bnm, a_nrm);

    // transform vertex to camera vector in model space into tangent space
    v_v = (u_camera_pos - a_pos.xyz) * m;
    // transform vertex to omniLS vector in model space into tangent space
    v_l = (u_omniLS_pos - a_pos.xyz) * m;
    // transform world up vector in model space into tangent space
    v_u = (u_up_dir) * m;

    vec3 distances;
    distances.x = 1.0;
    distances.y = length(v_l);
    distances.z = distances.y * distances.y;
    v_a = 1.0 / dot(u_attnCoeffs, distances);

    v_t = a_tex;
}

