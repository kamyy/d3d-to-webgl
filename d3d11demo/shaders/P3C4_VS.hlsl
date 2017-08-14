#pragma pack_matrix( row_major )

cbuffer VS_REG_B1 : register(b1) { // per model
    float4x4 g_model_view_proj_matrix; // model to view to projection space transform
    float4 g_camera_pos; // camera pos in model space
    float4 g_omniLS_pos; // omniLS pos in model space
    float4 g_up_dir; // world up in model space
};

struct VS_INPUT {
    float3 p : POSITION; // model space vertex position
    float4 c : COLOR; // vertex color
};

struct VS_OUTPUT {
    float4 p : SV_POSITION; // clip space vertex position
    float4 c : COLOR; // vertex color
};

void main(VS_INPUT vs_input, out VS_OUTPUT vs_output) {
    vs_output.p = mul(float4(vs_input.p, 1.0f), g_model_view_proj_matrix);
    vs_output.c = vs_input.c;
}
