#pragma pack_matrix( row_major )

cbuffer PS_REG_B0 : register(b0) { // per tick
    float4 g_intensity; // omni light source intensity
    float4 g_gnd; // ground hemisphere light intensity
    float4 g_sky; // sky hemisphere light intensity
};

cbuffer PS_REG_B1 : register(b1) { // per model
    float4 g_up_dir; // world up in model space
};

cbuffer PS_REG_B2 : register(b2) { // per material
    float4 g_a; // material ambient color
    float4 g_d; // omni light intensity * material diff color
    float4 g_s; // omni light intensity * material spec color and material shininess
};

struct PS_INPUT {
    float4 p : SV_POSITION; // interpolated clip space vertex position
    float3 n : TEXCOORD0; // model space interpolated vertex normal + hemisphere scalar
    float3 v : TEXCOORD1; // model space interpolated vertex to camera vector
    float3 l : TEXCOORD2; // model space interpolated vertex to omniLS vector
    float a : TEXCOORD3; // interpolated attenuation
};

void main(PS_INPUT ps_input, out float4 c : SV_TARGET) {
    float3 n = normalize(ps_input.n);
    float3 v = normalize(ps_input.v);
    float3 l = normalize(ps_input.l);

    float n_dot_l = saturate(dot(n, l));
    float v_dot_r = saturate(dot(v, 2.f * n_dot_l * n - l));
    float self_shadow = saturate(4.f * n_dot_l);

    float4 ambient = g_a * lerp(g_gnd, g_sky, dot(n, g_up_dir.xyz) * .5f + .5f);
    float4 diffuse = g_d * n_dot_l;

    c = ambient + self_shadow * ps_input.a * (diffuse + g_s * pow(v_dot_r, g_s.a));
    c.a = 1.0f;
}
