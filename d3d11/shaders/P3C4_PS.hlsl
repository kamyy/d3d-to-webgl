#pragma pack_matrix( row_major )

struct PS_INPUT {
    float4 p : SV_POSITION; // interpolated clip space vertex position
    float4 c : COLOR; // interpolated vertex color
};

float4 main(PS_INPUT ps_input) : SV_TARGET {
    return ps_input.c;
}
