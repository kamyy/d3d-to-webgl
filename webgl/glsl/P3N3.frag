precision highp float;

uniform vec3 u_gnd; // ground hemisphere light intensity
uniform vec3 u_sky; // sky hemisphere light intensity

uniform vec3 u_ambi; // material ambient color
uniform vec3 u_diff; // omni light intensity * material diff color
uniform vec4 u_spec; // omni light intensity * material spec color and material shininess

uniform vec3 u_up_dir; // world up in model space

varying vec3  v_n; // model space interpolated vertex normal + hemisphere scalar
varying vec3  v_v; // model space interpolated vertex to camera vector
varying vec3  v_l; // model space interpolated vertex to omniLS vector
varying float v_a; // interpolated attenuation

void main() {
    vec3 n = normalize(v_n);
    vec3 v = normalize(v_v);
    vec3 l = normalize(v_l);

    float n_dot_l = clamp(dot(n, l), 0.0, 1.0);
    float v_dot_r = clamp(dot(v, 2.0 * n_dot_l * n - l), 0.0, 1.0);
    float self_shadow = clamp(4.0 * n_dot_l, 0.0, 1.0);

    vec3 ambient = u_ambi * mix(u_gnd, u_sky, dot(n, u_up_dir) * 0.5 + 0.5);
    vec3 diffuse = u_diff * n_dot_l;

    gl_FragColor.xyz = ambient + (self_shadow * v_a) * (diffuse + u_spec.xyz * pow(v_dot_r, u_spec.a));
    gl_FragColor.a   = 1.0;
}

