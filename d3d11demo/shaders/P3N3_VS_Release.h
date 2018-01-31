#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer VS_REG_B0
// {
//
//   float4 g_att_coeff;                // Offset:    0 Size:    16
//
// }
//
// cbuffer VS_REG_B1
// {
//
//   row_major float4x4 g_model_view_proj_matrix;// Offset:    0 Size:    64
//   float4 g_camera_pos;               // Offset:   64 Size:    16
//   float4 g_omniLS_pos;               // Offset:   80 Size:    16
//   float4 g_up_dir;                   // Offset:   96 Size:    16 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// VS_REG_B0                         cbuffer      NA          NA            cb0      1 
// VS_REG_B1                         cbuffer      NA          NA            cb1      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// NORMAL                   0   xyz         1     NONE   float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xyz         1     NONE   float   xyz 
// TEXCOORD                 3      w        1     NONE   float      w
// TEXCOORD                 1   xyz         2     NONE   float   xyz 
// TEXCOORD                 2   xyz         3     NONE   float   xyz 
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c1         cb0             0         1  ( FLT, FLT, FLT, FLT)
// c2         cb1             0         6  ( FLT, FLT, FLT, FLT)
//
//
// Runtime generated constant mappings:
//
// Target Reg                               Constant Description
// ---------- --------------------------------------------------
// c0                              Vertex Shader position offset
//
//
// Level9 shader bytecode:
//
    vs_2_0
    def c8, 1, 0, 0, 0
    dcl_texcoord v0
    dcl_texcoord1 v1
    add oT1.xyz, -v0, c6
    add r0.xyz, -v0, c7
    dp3 r0.w, r0, r0
    mov oT2.xyz, r0
    rsq r0.x, r0.w
    rcp r0.y, r0.x
    mul r0.z, r0.y, r0.y
    mov r0.x, c8.x
    dp3 r0.x, c1, r0
    rcp oT0.w, r0.x
    mul r0, v0.y, c3
    mad r0, v0.x, c2, r0
    mad r0, v0.z, c4, r0
    add r0, r0, c5
    mad oPos.xy, r0.w, c0, r0
    mov oPos.zw, r0
    mov oT0.xyz, v1

// approximately 17 instruction slots used
vs_4_0
dcl_constantbuffer CB0[1], immediateIndexed
dcl_constantbuffer CB1[6], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xyz
dcl_output_siv o0.xyzw, position
dcl_output o1.xyz
dcl_output o1.w
dcl_output o2.xyz
dcl_output o3.xyz
dcl_temps 2
mul r0.xyzw, v0.yyyy, cb1[1].xyzw
mad r0.xyzw, v0.xxxx, cb1[0].xyzw, r0.xyzw
mad r0.xyzw, v0.zzzz, cb1[2].xyzw, r0.xyzw
add o0.xyzw, r0.xyzw, cb1[3].xyzw
mov r0.x, l(1.000000)
add r1.xyz, -v0.xyzx, cb1[5].xyzx
dp3 r0.z, r1.xyzx, r1.xyzx
mov o3.xyz, r1.xyzx
sqrt r0.y, r0.z
dp3 r0.x, cb0[0].xyzx, r0.xyzx
div o1.w, l(1.000000, 1.000000, 1.000000, 1.000000), r0.x
mov o1.xyz, v1.xyzx
add o2.xyz, -v0.xyzx, cb1[4].xyzx
ret 
// Approximately 14 instruction slots used
#endif

const BYTE g_P3N3_VS[] =
{
     68,  88,  66,  67, 214, 174, 
    253,  31,  29, 218, 215, 143, 
    237, 143, 140, 172,  61,   8, 
     49,   7,   1,   0,   0,   0, 
    248,   6,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    184,   1,   0,   0, 216,   3, 
      0,   0,  84,   4,   0,   0, 
      8,   6,   0,   0,  88,   6, 
      0,   0,  65, 111, 110,  57, 
    120,   1,   0,   0, 120,   1, 
      0,   0,   0,   2, 254, 255, 
     56,   1,   0,   0,  64,   0, 
      0,   0,   2,   0,  36,   0, 
      0,   0,  60,   0,   0,   0, 
     60,   0,   0,   0,  36,   0, 
      1,   0,  60,   0,   0,   0, 
      0,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   6,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   2, 254, 255, 
     81,   0,   0,   5,   8,   0, 
     15, 160,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   0,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      1, 128,   1,   0,  15, 144, 
      2,   0,   0,   3,   1,   0, 
      7, 224,   0,   0, 228, 145, 
      6,   0, 228, 160,   2,   0, 
      0,   3,   0,   0,   7, 128, 
      0,   0, 228, 145,   7,   0, 
    228, 160,   8,   0,   0,   3, 
      0,   0,   8, 128,   0,   0, 
    228, 128,   0,   0, 228, 128, 
      1,   0,   0,   2,   2,   0, 
      7, 224,   0,   0, 228, 128, 
      7,   0,   0,   2,   0,   0, 
      1, 128,   0,   0, 255, 128, 
      6,   0,   0,   2,   0,   0, 
      2, 128,   0,   0,   0, 128, 
      5,   0,   0,   3,   0,   0, 
      4, 128,   0,   0,  85, 128, 
      0,   0,  85, 128,   1,   0, 
      0,   2,   0,   0,   1, 128, 
      8,   0,   0, 160,   8,   0, 
      0,   3,   0,   0,   1, 128, 
      1,   0, 228, 160,   0,   0, 
    228, 128,   6,   0,   0,   2, 
      0,   0,   8, 224,   0,   0, 
      0, 128,   5,   0,   0,   3, 
      0,   0,  15, 128,   0,   0, 
     85, 144,   3,   0, 228, 160, 
      4,   0,   0,   4,   0,   0, 
     15, 128,   0,   0,   0, 144, 
      2,   0, 228, 160,   0,   0, 
    228, 128,   4,   0,   0,   4, 
      0,   0,  15, 128,   0,   0, 
    170, 144,   4,   0, 228, 160, 
      0,   0, 228, 128,   2,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 128,   5,   0, 
    228, 160,   4,   0,   0,   4, 
      0,   0,   3, 192,   0,   0, 
    255, 128,   0,   0, 228, 160, 
      0,   0, 228, 128,   1,   0, 
      0,   2,   0,   0,  12, 192, 
      0,   0, 228, 128,   1,   0, 
      0,   2,   0,   0,   7, 224, 
      1,   0, 228, 144, 255, 255, 
      0,   0,  83,  72,  68,  82, 
     24,   2,   0,   0,  64,   0, 
      1,   0, 134,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      1,   0,   0,   0,   6,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   0,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   1,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 130,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   2,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   3,   0,   0,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  86,  21,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0,   6,  16, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   8, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  54,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,  18, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,  16,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    114,  32,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  75,   0, 
      0,   5,  34,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     16,   0,   0,   8,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  14,   0,   0,  10, 
    130,  32,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0, 128,  63,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 114,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   9, 
    114,  32,  16,   0,   2,   0, 
      0,   0,  70,  18,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 116,   0, 
      0,   0,  14,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     10,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  82,  68, 
     69,  70, 172,   1,   0,   0, 
      2,   0,   0,   0, 112,   0, 
      0,   0,   2,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    254, 255,   0,   1,   0,   0, 
    131,   1,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    102,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  86,  83,  95,  82, 
     69,  71,  95,  66,  48,   0, 
     86,  83,  95,  82,  69,  71, 
     95,  66,  49,   0,  92,   0, 
      0,   0,   1,   0,   0,   0, 
    160,   0,   0,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 102,   0, 
      0,   0,   4,   0,   0,   0, 
    212,   0,   0,   0, 112,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 184,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 196,   0,   0,   0, 
      0,   0,   0,   0, 103,  95, 
     97, 116, 116,  95,  99, 111, 
    101, 102, 102,   0,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  52,   1,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
     80,   1,   0,   0,   0,   0, 
      0,   0,  96,   1,   0,   0, 
     64,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
    196,   0,   0,   0,   0,   0, 
      0,   0, 109,   1,   0,   0, 
     80,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
    196,   0,   0,   0,   0,   0, 
      0,   0, 122,   1,   0,   0, 
     96,   0,   0,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
    196,   0,   0,   0,   0,   0, 
      0,   0, 103,  95, 109, 111, 
    100, 101, 108,  95, 118, 105, 
    101, 119,  95, 112, 114, 111, 
    106,  95, 109,  97, 116, 114, 
    105, 120,   0, 171, 171, 171, 
      2,   0,   3,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 103,  95, 
     99,  97, 109, 101, 114,  97, 
     95, 112, 111, 115,   0, 103, 
     95, 111, 109, 110, 105,  76, 
     83,  95, 112, 111, 115,   0, 
    103,  95, 117, 112,  95, 100, 
    105, 114,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  49,  48,  46,  49, 
      0, 171,  73,  83,  71,  78, 
     72,   0,   0,   0,   2,   0, 
      0,   0,   8,   0,   0,   0, 
     56,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   7,   7,   0,   0, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   7,   7,   0,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  78,  79,  82, 
     77,  65,  76,   0,  79,  83, 
     71,  78, 152,   0,   0,   0, 
      5,   0,   0,   0,   8,   0, 
      0,   0, 128,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0, 140,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   7,   8, 
      0,   0, 140,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   8,   7, 
      0,   0, 140,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   7,   8, 
      0,   0, 140,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,   7,   8, 
      0,   0,  83,  86,  95,  80, 
     79,  83,  73,  84,  73,  79, 
     78,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0, 171, 
    171, 171
};