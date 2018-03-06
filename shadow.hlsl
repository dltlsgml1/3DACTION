
float4x4 g_world;
float4x4 g_view;
float4x4 g_projection;

bool g_tex; // テクスチャのありなし　false:なし　true:あり

// 光
float4 g_diffuse; // ディフューズ
float4 g_emmisive; // エミッシブ
float4 g_ambient; // 環境光
float4 g_specular; // スペキュラー光

float3 g_light_dir; // 平行光源の方向

// マテリアル
float4 g_diffuse_mat; // ディフューズ光
float4 g_emmisive_mat; // エミッシブ光
float4 g_ambient_mat; // 環境光
float4 g_specular_mat; // スペキュラー
float g_power; // スペキュラー光のパワー値

// スペキュラ光の計算用
float4 g_camerapos; // カメラ位置



void VS(float3 in_pos : POSITION,
    out float4 out_pos : POSITION,
    out float4 out_depth : TEXCOORD0)
{
    out_pos = mul(float4(in_pos, 1.0f), g_world);
    out_pos = mul(out_pos, g_view);
    out_pos = mul(out_pos, g_projection);
    out_depth = out_pos;
}

void PS(float4 in_depth : TEXCOORD0,
    out float4 out_color : COLOR0)
{
    out_color = in_depth.z / in_depth.w;
}