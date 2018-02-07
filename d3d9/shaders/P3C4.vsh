float4x4 g_world_view_proj_matrix; // model to view to projection space transform

struct VS_INPUT {
	float4 p : POSITION;// model space vertex position
	float4 c : COLOR;	// vertex color
};

struct VS_OUTPUT {
	float4 p : POSITION;// clip space vertex position
	float4 c : COLOR;	// vertex color
};

void main(VS_INPUT vs_input, out VS_OUTPUT vs_output) {
	vs_output.p = mul(vs_input.p, g_world_view_proj_matrix);
	vs_output.c = vs_input.c;
}
