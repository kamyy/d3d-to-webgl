#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
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
//   float4 g_up_dir;                   // Offset:   96 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// VS_REG_B0                         cbuffer      NA          NA    0        1
// VS_REG_B1                         cbuffer      NA          NA    1        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// NORMAL                   0   xyz         1     NONE   float   xyz 
// BINORMAL                 0   xyz         2     NONE   float   xyz 
// TEXCOORD                 0   xy          3     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xyz         1     NONE   float   xyz 
// TEXCOORD                 1   xyz         2     NONE   float   xyz 
// TEXCOORD                 2   xyz         3     NONE   float   xyz 
// TEXCOORD                 3   xy          4     NONE   float   xy  
// TEXCOORD                 4     z         4     NONE   float     z 
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c1         cb0             0         1  ( FLT, FLT, FLT, FLT)
// c2         cb1             0         7  ( FLT, FLT, FLT, FLT)
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
    def c9, 1, 0, 0, 0
    dcl_texcoord v0
    dcl_texcoord1 v1
    dcl_texcoord2 v2
    dcl_texcoord3 v3
    mov r0.xyz, v2
    mul r1.xyz, r0.zxyw, v1.yzxw
    mad r0.xyz, r0.yzxw, v1.zxyw, -r1
    add r1.xyz, -v0, c6
    dp3 oT0.x, r0, r1
    dp3 oT0.y, v2, r1
    dp3 oT0.z, v1, r1
    add r1.xyz, -v0, c7
    dp3 oT1.x, r0, r1
    dp3 oT2.x, r0, c8
    dp3 oT1.y, v2, r1
    dp3 oT1.z, v1, r1
    dp3 r0.x, r1, r1
    rsq r0.x, r0.x
    rcp r0.y, r0.x
    dp3 oT2.y, v2, c8
    dp3 oT2.z, v1, c8
    mul r0.z, r0.y, r0.y
    mov r0.x, c9.x
    dp3 r0.x, c1, r0
    rcp oT3.z, r0.x
    mul r0, v0.y, c3
    mad r0, v0.x, c2, r0
    mad r0, v0.z, c4, r0
    add r0, r0, c5
    mad oPos.xy, r0.w, c0, r0
    mov oPos.zw, r0
    mov oT3.xy, v3

// approximately 28 instruction slots used
vs_4_0
dcl_constantbuffer cb0[1], immediateIndexed
dcl_constantbuffer cb1[7], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xyz
dcl_input v2.xyz
dcl_input v3.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xyz
dcl_output o2.xyz
dcl_output o3.xyz
dcl_output o4.xy
dcl_output o4.z
dcl_temps 2
mul r0.xyzw, v0.yyyy, cb1[1].xyzw
mad r0.xyzw, v0.xxxx, cb1[0].xyzw, r0.xyzw
mad r0.xyzw, v0.zzzz, cb1[2].xyzw, r0.xyzw
add o0.xyzw, r0.xyzw, cb1[3].xyzw
mul r0.xyz, v1.yzxy, v2.zxyz
mad r0.xyz, v2.yzxy, v1.zxyz, -r0.xyzx
add r1.xyz, -v0.xyzx, cb1[4].xyzx
dp3 o1.x, r0.xyzx, r1.xyzx
dp3 o1.y, v2.xyzx, r1.xyzx
dp3 o1.z, v1.xyzx, r1.xyzx
add r1.xyz, -v0.xyzx, cb1[5].xyzx
dp3 o2.x, r0.xyzx, r1.xyzx
dp3 o3.x, r0.xyzx, cb1[6].xyzx
dp3 o2.y, v2.xyzx, r1.xyzx
dp3 o2.z, v1.xyzx, r1.xyzx
dp3 r0.z, r1.xyzx, r1.xyzx
dp3 o3.y, v2.xyzx, cb1[6].xyzx
dp3 o3.z, v1.xyzx, cb1[6].xyzx
sqrt r0.y, r0.z
mov r0.x, l(1.000000)
dp3 r0.x, cb0[0].xyzx, r0.xyzx
div o4.z, l(1.000000, 1.000000, 1.000000, 1.000000), r0.x
mov o4.xy, v3.xyxx
ret 
// Approximately 24 instruction slots used
#endif

const BYTE g_P3N3B3T2_VS[] =
{
     68,  88,  66,  67, 235,  81, 
     81, 117,  80,  24, 188,  16, 
    234,   9, 246, 186, 110, 179, 
    224,  23,   1,   0,   0,   0, 
    136,   9,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    132,   2,   0,   0,   0,   6, 
      0,   0, 124,   6,   0,   0, 
     60,   8,   0,   0, 208,   8, 
      0,   0,  65, 111, 110,  57, 
     68,   2,   0,   0,  68,   2, 
      0,   0,   0,   2, 254, 255, 
      4,   2,   0,   0,  64,   0, 
      0,   0,   2,   0,  36,   0, 
      0,   0,  60,   0,   0,   0, 
     60,   0,   0,   0,  36,   0, 
      1,   0,  60,   0,   0,   0, 
      0,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   7,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   2, 254, 255, 
     81,   0,   0,   5,   9,   0, 
     15, 160,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   0,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      1, 128,   1,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      2, 128,   2,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      3, 128,   3,   0,  15, 144, 
      1,   0,   0,   2,   0,   0, 
      7, 128,   2,   0, 228, 144, 
      5,   0,   0,   3,   1,   0, 
      7, 128,   0,   0, 210, 128, 
      1,   0, 201, 144,   4,   0, 
      0,   4,   0,   0,   7, 128, 
      0,   0, 201, 128,   1,   0, 
    210, 144,   1,   0, 228, 129, 
      2,   0,   0,   3,   1,   0, 
      7, 128,   0,   0, 228, 145, 
      6,   0, 228, 160,   8,   0, 
      0,   3,   0,   0,   1, 224, 
      0,   0, 228, 128,   1,   0, 
    228, 128,   8,   0,   0,   3, 
      0,   0,   2, 224,   2,   0, 
    228, 144,   1,   0, 228, 128, 
      8,   0,   0,   3,   0,   0, 
      4, 224,   1,   0, 228, 144, 
      1,   0, 228, 128,   2,   0, 
      0,   3,   1,   0,   7, 128, 
      0,   0, 228, 145,   7,   0, 
    228, 160,   8,   0,   0,   3, 
      1,   0,   1, 224,   0,   0, 
    228, 128,   1,   0, 228, 128, 
      8,   0,   0,   3,   2,   0, 
      1, 224,   0,   0, 228, 128, 
      8,   0, 228, 160,   8,   0, 
      0,   3,   1,   0,   2, 224, 
      2,   0, 228, 144,   1,   0, 
    228, 128,   8,   0,   0,   3, 
      1,   0,   4, 224,   1,   0, 
    228, 144,   1,   0, 228, 128, 
      8,   0,   0,   3,   0,   0, 
      1, 128,   1,   0, 228, 128, 
      1,   0, 228, 128,   7,   0, 
      0,   2,   0,   0,   1, 128, 
      0,   0,   0, 128,   6,   0, 
      0,   2,   0,   0,   2, 128, 
      0,   0,   0, 128,   8,   0, 
      0,   3,   2,   0,   2, 224, 
      2,   0, 228, 144,   8,   0, 
    228, 160,   8,   0,   0,   3, 
      2,   0,   4, 224,   1,   0, 
    228, 144,   8,   0, 228, 160, 
      5,   0,   0,   3,   0,   0, 
      4, 128,   0,   0,  85, 128, 
      0,   0,  85, 128,   1,   0, 
      0,   2,   0,   0,   1, 128, 
      9,   0,   0, 160,   8,   0, 
      0,   3,   0,   0,   1, 128, 
      1,   0, 228, 160,   0,   0, 
    228, 128,   6,   0,   0,   2, 
      3,   0,   4, 224,   0,   0, 
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
      0,   2,   3,   0,   3, 224, 
      3,   0, 228, 144, 255, 255, 
      0,   0,  83,  72,  68,  82, 
    116,   3,   0,   0,  64,   0, 
      1,   0, 221,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      1,   0,   0,   0,   7,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   0,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   1,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   2,   0, 
      0,   0,  95,   0,   0,   3, 
     50,  16,  16,   0,   3,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   2,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   3,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   4,   0,   0,   0, 
    101,   0,   0,   3,  66,  32, 
     16,   0,   4,   0,   0,   0, 
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
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 150,  20,  16,   0, 
      1,   0,   0,   0,  38,  25, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    150,  20,  16,   0,   2,   0, 
      0,   0,  38,  25,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,  18, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,  16,   0, 
      0,   7,  18,  32,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  16,   0,   0,   7, 
     34,  32,  16,   0,   1,   0, 
      0,   0,  70,  18,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     16,   0,   0,   7,  66,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,  18, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,  16,   0, 
      0,   7,  18,  32,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  16,   0,   0,   8, 
     18,  32,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      6,   0,   0,   0,  16,   0, 
      0,   7,  34,  32,  16,   0, 
      2,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  16,   0,   0,   7, 
     66,  32,  16,   0,   2,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     16,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  16,   0, 
      0,   8,  34,  32,  16,   0, 
      3,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   6,   0,   0,   0, 
     16,   0,   0,   8,  66,  32, 
     16,   0,   3,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,   6,   0, 
      0,   0,  75,   0,   0,   5, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     16,   0,   0,   8,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  14,   0,   0,  10, 
     66,  32,  16,   0,   4,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0, 128,  63,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  50,  32, 
     16,   0,   4,   0,   0,   0, 
     70,  16,  16,   0,   3,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 116,   0, 
      0,   0,  24,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,  10,   0,   0,   0, 
     21,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  82,  68, 
     69,  70, 184,   1,   0,   0, 
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
      0,   0,   2,   0,   0,   0, 
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
    114,  32,  54,  46,  51,  46, 
     57,  54,  48,  48,  46,  49, 
     54,  51,  56,  52,   0, 171, 
    171, 171,  73,  83,  71,  78, 
    140,   0,   0,   0,   4,   0, 
      0,   0,   8,   0,   0,   0, 
    104,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   7,   7,   0,   0, 
    113,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   7,   7,   0,   0, 
    120,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   7,   7,   0,   0, 
    129,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   3,   3,   0,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  78,  79,  82, 
     77,  65,  76,   0,  66,  73, 
     78,  79,  82,  77,  65,  76, 
      0,  84,  69,  88,  67,  79, 
     79,  82,  68,   0, 171, 171, 
     79,  83,  71,  78, 176,   0, 
      0,   0,   6,   0,   0,   0, 
      8,   0,   0,   0, 152,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 164,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   8,   0,   0, 164,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   8,   0,   0, 164,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      7,   8,   0,   0, 164,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
      3,  12,   0,   0, 164,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
      4,  11,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0, 171, 171, 171
};
