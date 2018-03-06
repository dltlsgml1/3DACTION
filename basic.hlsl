
 
float4x4 g_world;
float4x4 g_view;
float4x4 g_projection;

float4x4 g_matuv;

sampler Sampler1;
sampler ToonSampler1;
sampler NormalSampler;
sampler ShadowSampler;


float4 g_light_dir;
float4 g_camera_pos;
float4 g_specular;

float4 g_ambient_material;
float4 g_diffuse_material;
float4 g_specular_material;
bool istheretexture = false;
bool drawguideline = false;

// 光源の位置にカメラを置いたカメラ行列
float4x4    g_lightposcamera;
float4x4	g_lightposprojection;


void VS(float3 in_pos : POSITION,
        float2 in_tex : TEXCOORD0,
        float3 in_normal : NORMAL,
    out float4 out_pos : POSITION,
    out float2 out_tex : TEXCOORD0,
    out float3 out_normal : TEXCOORD1,
    out float3 out_posforps : TEXCOORD2,
    out float4 out_shadowtexuv:TEXCOORD3,
    out float4 out_lengthfromlight:TEXCOORD4)
{
    float3 N;
    if (drawguideline)
    {
        N = normalize(in_normal);
        in_pos = in_pos + N * 0.01;
    }
    else
    {
        N = mul(in_normal, (float3x3) g_world);
    }
    out_pos = mul(float4(in_pos, 1.0f), g_world);
    out_posforps = out_pos.xyz;
    out_pos = mul(out_pos, g_view);
    out_pos = mul(out_pos, g_projection);

   // シャドウテクスチャ用のＵＶを作成する
    float4 shadowtexpos;
    shadowtexpos = mul(float4(in_pos, 1.0f), g_world);
    shadowtexpos = mul(shadowtexpos, g_lightposcamera);
    shadowtexpos = mul(shadowtexpos, g_lightposprojection);
    out_lengthfromlight = shadowtexpos; // 光源からの距離をピクセルシェーダーに渡す

    shadowtexpos = mul(shadowtexpos, g_matuv);
    out_shadowtexuv = shadowtexpos;

    out_tex = in_tex;



    out_normal = normalize(mul(in_normal, (float3x3) g_world));
}



void PS(float3 in_normal : TEXCOORD1,
        float2 in_tex : TEXCOORD0,
        float3 in_pos : TEXCOORD2,
        float4 in_shadowtexuv : TEXCOORD3,
        float4 in_lengthfromlight : TEXCOORD4,
    out float4 out_color : COLOR0)
{
    float4 texcol = tex2D(Sampler1, in_tex);
   
    float3 N,L,V,H,P;
  
    P = in_pos.xyz;
    N = normalize(in_normal);
    L = normalize(-g_light_dir);
    V = normalize(g_camera_pos.xyz - P);
    H = L + V;
    H = normalize(H);
    float4 diffuse = max(0.0f, dot(L, N));
    float4 diffusecolor = g_diffuse_material;
    float4 specular = pow(max(0.0f, dot(N, H)), 500);

    float2 toonuv;
    toonuv.x = diffuse;
    toonuv.y = diffuse;
    float4 tooncolor = tex2D(ToonSampler1, toonuv);

  
    if (istheretexture)
    {
        out_color =  diffusecolor * texcol + specular;
    }
    else 
    {
        out_color =   diffusecolor + specular;

    }
    float2 shadowtex = 0.5 * in_lengthfromlight.xy / in_lengthfromlight.w + float2(0.5, 0.5);
    shadowtex.y = 1.0f - shadowtex.y;

    float4 shadowmapz = tex2D(ShadowSampler, shadowtex);


    if (shadowtex.x <= 1.0f)
    {
        if (shadowtex.y <= 1.0f)
        {
            if (shadowtex.x >= 0.0f)
            {
                if (shadowtex.y >= 0.0f)
                {
                    float LengthFromLight = in_lengthfromlight.z / in_lengthfromlight.w;
                    if (LengthFromLight - shadowmapz.x > 0.0007f)
                    {
                        out_color = 0.5 * out_color;
                    }
                }
            }
        }
    }

    if(drawguideline)
    {
        out_color = float4(1, 0, 0, 1);
    }
    
}
