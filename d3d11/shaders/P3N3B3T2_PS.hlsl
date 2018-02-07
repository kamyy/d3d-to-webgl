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

Texture2D    diffuse_texture;
SamplerState diffuse_texture_sampler_state;

Texture2D    normal_texture;
SamplerState normal_texture_sampler_state;

struct PS_INPUT {
    float4 p : SV_POSITION; // interpolated clip space vertex position
    float3 v : TEXCOORD0; // interpolated tangent space vertex to camera vector
    float3 l : TEXCOORD1; // interpolated tangent space vertex to spotLS vector
    float3 u : TEXCOORD2; // interpolated tangent space world up vector
    float2 t : TEXCOORD3; // interpolated texture coordintes
    float a : TEXCOORD4; // interpolated attenuation
};

void main(PS_INPUT ps_input, out float4 c : SV_TARGET) {
    float3 v = normalize(ps_input.v);
    float3 l = normalize(ps_input.l);
    float3 u = normalize(ps_input.u);
    float4 d = diffuse_texture.Sample(diffuse_texture_sampler_state, ps_input.t); // diffuse map
    float3 n = normalize(normal_texture.Sample(normal_texture_sampler_state, ps_input.t) * 2.f - 1.f).xyz; // normal map

    float n_dot_l = saturate(dot(n, l));
    float v_dot_r = saturate(dot(v, 2.f * n_dot_l * n - l));
    float self_shadow = saturate(4.f * l.z);

    float4 ambient = d * lerp(g_gnd, g_sky, dot(n, u) * .5f + .5f);
    float4 diffuse = d * g_intensity * n_dot_l;

    c = ambient + self_shadow * ps_input.a * (diffuse + g_s * pow(v_dot_r, g_s.a));
    c.a = d.a;
}
