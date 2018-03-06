
 
float4x4 g_world;
float4x4 g_view;
float4x4 g_projection;
float4x4 g_matuv;

sampler Sampler1;
sampler ToonSampler1;
sampler NormalSampler;
sampler ShadowSampler;

float4 g_light_dir;
float4 g_inv_light_dir;
float4 g_camera_pos;
float4 g_inv_camera_pos;
float4 g_specular;
float4 g_inv_pos;

float4 g_ambient_material;
float4 g_diffuse_material;
float4 g_specular_material;

// �����̈ʒu�ɃJ������u�����J�����s��
float4x4 g_lightposcamera;
float4x4 g_lightposprojection;


void VS(float3 in_pos : POSITION,
	float3 in_normal : NORMAL,          //N
	float2 in_tex1 : TEXCOORD0,
	float3 in_binormal : BINORMAL,      //B
	float3 in_tangent : TANGENT,        //T
	out float4 out_pos : POSITION,
	out float3 out_pos2 : TEXCOORD4,
	out float2 out_tex1 : TEXCOORD0,
	out float3 out_lightvec : TEXCOORD1,
	out float3 out_normal : TEXCOORD2,
	out float3 out_camerapos : TEXCOORD3,
    out float4 out_shadowtexuv :TEXCOORD5,
    out float4 out_lengthfromlight:TEXCOORD6
)
{
	float3	N;		// ���[���h��ԏ�̖@���x�N�g��
	float3	L;		// ���̍������ޕ���
	float3	P;		// ���[���h��ԏ�̒��_���W
	float3	V;		// �i�J�������W�[���_���W�j�x�N�g��
	float3  R;      // �i�J�������W�[���_���W�j�x�N�g����@���x�N�g�������ɔ��˂�����
	float3  H;		//  �n�[�t�x�N�g���i�����x�N�g���ƌ��̕����x�N�g���j

					// ���W�ϊ�
	out_pos = mul(float4(in_pos, 1.0f), g_world);
	out_pos = mul(out_pos, g_view);
	out_pos = mul(out_pos, g_projection);
	

	//  P = out_pos.xyz;

 
	P = in_pos.xyz;
	float3 sP;
	sP.x = dot(P, in_tangent);
	sP.y = dot(P, in_binormal);
	sP.z = dot(P, in_normal);

	// ���s���̍������ޕ���	�P�ʃx�N�g����
	L = normalize(-g_inv_light_dir);
	float3 sL;
	sL.x = dot(L, in_tangent);
	sL.y = dot(L, in_binormal);
	sL.z = dot(L, in_normal);

	float3 camerapos = g_inv_camera_pos.xyz;
	float3 sCPos;
	sCPos.x = dot(camerapos, in_tangent);
	sCPos.y = dot(camerapos, in_binormal);
	sCPos.z = dot(camerapos, in_normal);

    // �V���h�E�e�N�X�`���p�̂t�u���쐬����
    float4 shadowtexpos;
    shadowtexpos = mul(float4(in_pos, 1.0f), g_world);
    shadowtexpos = mul(shadowtexpos, g_lightposcamera);
    shadowtexpos = mul(shadowtexpos, g_lightposprojection);
    out_lengthfromlight = shadowtexpos; // ��������̋������s�N�Z���V�F�[�_�[�ɓn��

    shadowtexpos = mul(shadowtexpos, g_matuv);
    out_shadowtexuv = shadowtexpos;


	// �e�N�X�`�����W�����̂܂܏o�͂���
	out_tex1 = in_tex1;
	out_pos2 = sP;
	out_lightvec = sL;
	out_camerapos = sCPos;

}


void PS(
	float2 in_tex1 : TEXCOORD0,
	float3 in_lightvec : TEXCOORD1,
	float3 in_normal : TEXCOORD2,
	float3 in_pos : TEXCOORD4,
	float3 in_camerapos : TEXCOORD3,
    float4 in_shadowtexuv:TEXCOORD5,
    float4 in_lengthfromlight : TEXCOORD6,
	out float4 out_color : COLOR0)
{
	float3 L, P, CP, V, H;

	P = in_pos.xyz;

	// �@��
	float4 N = tex2D(NormalSampler, in_tex1);
	N = (N * 2.0f) - 1.0f;
	N = normalize(N);

	// hikari
	L = normalize(in_lightvec);

	CP = in_camerapos;

	V = CP.xyz - P;

	H = normalize(L + V);
	// lambert
	float d = max(0.0f, (dot(L, -N.xyz)));
	float4 tex = tex2D(Sampler1, in_tex1);
	float4 specular = pow(max(0.0f, dot(N.xyz, H)), 5.0f);
	float4 diffusecolor = tex *d;


    
    out_color = diffusecolor+specular;

    // �t�u���W�̌v�Z
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



}
