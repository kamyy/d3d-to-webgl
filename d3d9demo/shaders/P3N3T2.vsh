float4x4 g_world_view_proj_matrix; // model to view to projection space transform
float3 g_camera_pos; // camera pos in model space
float3 g_omniLS_pos; // omniLS pos in model space
float3 g_att_coeffs; // attenuation coefficients

struct VS_INPUT {
	float4 p : POSITION;
	float3 n : NORMAL;
	float2 t : TEXCOORD;
};

struct VS_OUTPUT {
	float4 p : POSITION; // clip space vertex position
	float3 n : TEXCOORD0; // model space vertex normal
	float3 v : TEXCOORD1; // model space vertex to camera vector
	float3 l : TEXCOORD2; // model space vertex to omniLS vector
	float2 t : TEXCOORD3; // vertex texture coordinates
	float a : TEXCOORD4; // attenuation
};

void main(VS_INPUT vs_input, out VS_OUTPUT vs_output) {
	vs_output.p = mul(vs_input.p, g_world_view_proj_matrix);
	vs_output.n = vs_input.n;
	vs_output.v = g_camera_pos - vs_input.p;
	vs_output.l = g_omniLS_pos - vs_input.p;
	vs_output.t = vs_input.t;

	float3 distances;
	distances.x = 1.f;
	distances.y = length(vs_output.l);
	distances.z = distances.y * distances.y;
	vs_output.a = 1.0f / dot(g_att_coeffs, distances);
}
