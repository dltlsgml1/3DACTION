
float4x4 g_world;
float4x4 g_view;
float4x4 g_projection;

bool g_tex; // �e�N�X�`���̂���Ȃ��@false:�Ȃ��@true:����

// ��
float4 g_diffuse; // �f�B�t���[�Y
float4 g_emmisive; // �G�~�b�V�u
float4 g_ambient; // ����
float4 g_specular; // �X�y�L�����[��

float3 g_light_dir; // ���s�����̕���

// �}�e���A��
float4 g_diffuse_mat; // �f�B�t���[�Y��
float4 g_emmisive_mat; // �G�~�b�V�u��
float4 g_ambient_mat; // ����
float4 g_specular_mat; // �X�y�L�����[
float g_power; // �X�y�L�����[���̃p���[�l

// �X�y�L�������̌v�Z�p
float4 g_camerapos; // �J�����ʒu



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