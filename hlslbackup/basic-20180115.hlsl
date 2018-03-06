


float4x4 g_world;
float4x4 g_view;
float4x4 g_projection;

sampler Sampler1;
sampler ToonSampler1;

float4 g_light_dir;
float4 g_camera_pos;
float4 g_specular;

float4 g_ambient_material;
float4 g_diffuse_material;
float4 g_specular_material;
bool istheretexture = false;
bool drawguideline = false;

void VS(float3 in_pos : POSITION,
        float2 in_tex : TEXCOORD0,
        float3 in_normal : NORMAL,
    out float4 out_pos : POSITION,
    out float2 out_tex : TEXCOORD0,
    out float3 out_normal : TEXCOORD1,
    out float3 out_posforps : TEXCOORD2)
{
    float3 N;
    if (drawguideline)
    {
        N = normalize(in_normal);
        in_pos = in_pos + N * 0.05;
    }
    else
    {
        N = mul(in_normal, (float3x3) g_world);
    }
    out_pos = mul(float4(in_pos, 1.0f), g_world);
    out_pos = mul(out_pos, g_view);
    out_pos = mul(out_pos, g_projection);
    float3 P;
    P = out_pos.xyz;
    out_posforps = P;

    out_tex = in_tex;

    out_normal = normalize(mul(in_normal, (float3x3) g_world));
}



void PS(float3 in_normal : TEXCOORD1,
        float2 in_tex : TEXCOORD0,
        float3 in_pos : TEXCOORD2,
    out float4 out_color : COLOR0)
{
    float4 texcol = tex2D(Sampler1, in_tex);
   
    float3 N,L,V,H,P;
  
    P = in_pos.xyz;
    N = normalize(in_normal);
    L = normalize(-g_light_dir);
    V = g_camera_pos.xyz-P;
    H = L + V;
    H = normalize(H);
    float4 diffuse = max(0.0f, dot(L, N));
    float4 diffusecolor = g_diffuse_material;
    float4 specular = pow(max(0.0f, dot(N, H)), 300);

    float2 toonuv;
    toonuv.x = diffuse;
    toonuv.y = diffuse;
    float4 tooncolor = tex2D(ToonSampler1, toonuv);

  
    if (istheretexture)
    {
        out_color = diffusecolor * tooncolor * texcol + specular;
    }
    else 
    {
        out_color = diffusecolor * tooncolor + specular;    
    }

    if(drawguideline)
    {
        out_color = float4(1, 0, 0, 1);
    }

    out_color.a = 1.0f;
}
