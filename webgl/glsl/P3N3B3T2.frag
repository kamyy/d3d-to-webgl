precision highp float;

uniform vec3 u_int; // omniLS intensity
uniform vec3 u_gnd; // ground hemisphere light intensity
uniform vec3 u_sky; // sky hemisphere light intensity

uniform vec3      u_ambi; // material ambient color
uniform vec4      u_spec; // omni light intensity * material spec color and material shininess
uniform sampler2D u_smpl; // texture sampler
uniform sampler2D u_norm; // normal map sampler

varying vec3  v_v; // tangent space vertex to camera vector
varying vec3  v_l; // tangent space vertex to omniLS vector
varying vec3  v_u; // tangent space world up vector
varying float v_a; // OmniLS attenuation
varying vec2  v_t; // texture coords

void main() {
    vec3 v = normalize(v_v);
    vec3 l = normalize(v_l);
    vec3 u = normalize(v_u);

    vec4 d = texture2D(u_smpl, v_t);
    vec3 n = normalize(texture2D(u_norm, v_t).xyz * 2.0 - 1.0);

    float n_dot_l = clamp(dot(n, l), 0.0, 1.0);
    float v_dot_r = clamp(dot(v, 2.0 * n_dot_l * n - l), 0.0, 1.0);
    float self_shadow = clamp(4.0 * n_dot_l, 0.0, 1.0);

    vec3 ambient = u_ambi * mix(u_gnd, u_sky, dot(n, u) * 0.5 + 0.5);
    vec3 diffuse = d.xyz * u_int * n_dot_l;

    gl_FragColor.xyz = ambient + (self_shadow * v_a) * (diffuse + u_spec.xyz * pow(v_dot_r, u_spec.a));
    gl_FragColor.a   = d.a;
}

