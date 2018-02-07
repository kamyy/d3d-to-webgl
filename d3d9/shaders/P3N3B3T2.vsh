float4x4 g_world_view_proj_matrix; // model to view to projection space transform
float3 g_camera_pos; // camera pos in model space
float3 g_omniLS_pos; // omniLS pos in model space
float3 g_att_coeffs; // attenuation coefficients
float3 g_up_dir; // world space up in model space

struct VS_INPUT {
	float4 p : POSITION;
	float3 n : NORMAL;
	float3 b : BINORMAL;
	float2 t : TEXCOORD;
};

struct VS_OUTPUT {
	float4 p : POSITION; // clip space vertex position
	float3 v : TEXCOORD0; // tangent space vertex to camera vector
	float3 l : TEXCOORD1; // tangent space vertex to omniLS vector
	float3 u : TEXCOORD2; // tangent space up world vector
	float2 t : TEXCOORD3; // vertex texture coordinates
	float a : TEXCOORD4; // attenuation
};

void main(VS_INPUT vs_input, out VS_OUTPUT vs_output) {
	vs_output.p = mul(vs_input.p, g_world_view_proj_matrix);
	vs_output.t = vs_input.t;
	
	float3 t = cross(vs_input.b, vs_input.n); // tangent vector
	float3 v = (g_camera_pos - vs_input.p);
	float3 l = (g_omniLS_pos - vs_input.p);

	float3x3 m; // tangent space to model space transform (transpose is useful)
	m[0] = t;
	m[1] = vs_input.b;
	m[2] = vs_input.n;
	
	// transform vertex to camera vector in model space into tangent space
	vs_output.v = mul(m, v);
	// transform vertex to omniLS vector in model space into tangent space
	vs_output.l = mul(m, l);
	// transform world up vector in model space into tangent space	
	vs_output.u = mul(m, g_up_dir);
	
	float3 distances;
	distances.x = 1.f;
	distances.y = length(l);
	distances.z = distances.y * distances.y;
	vs_output.a = 1.0f / dot(g_att_coeffs, distances);
}
