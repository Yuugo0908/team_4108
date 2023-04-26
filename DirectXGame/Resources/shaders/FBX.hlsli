// �{�[���̍ő吔
static const int MAX_BONES = 32;

// �s��p�萔�o�b�t�@
cbuffer cbuff0 : register(b0)
{
	matrix viewproj;	// �r���[�v���W�F�N�V�����s��
	matrix world;		// ���[���h�s��
	float3 cameraPos;	// �J�������W
};
cbuffer skinning:register(b1)
{
	// �{�[���̃X�L�j���O�s��
	matrix matSkinning[MAX_BONES];
};
cbuffer cbuff1 : register(b2)
{
	float3 m_ambient;// : packoffset(c0); //�A���r�G���g�W��
	float3 m_diffuse;// : packoffset(c1); //�f�B�t���[�Y�W��
	float3 m_specular;// : packoffset(c2); //�X�y�L�����[�W��
	float m_alpha;// : packoffset(c2.w); //�A���t�@
};
cbuffer cbuff2 : register(b3)
{
	float3 lightVec;		// ���C�g�����̒P�ʃx�N�g��
	float3 lightColor;		// ���C�g�̐F(RGB)
}

// vertex�o�b�t�@�[�̓���
struct VSInput
{
	float4 pos : POSITION;	// �ʒu
	float3 normal : NORMAL;	// ���_�@��
	float2 uv : TEXCOORD;	//�e�N�X�`�����W
	uint4 boneIndices : BONEINDICES; // �{�[���̔ԍ�
	float4 boneWeights : BONEWEIGHTS; // �{�[���̃X�L���E�F�C�g
};
// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float4 worldpos : POSITION; //���[���h���W
	float3 normal : NORMAL;		// �@��
	float2 uv : TEXCOORD;		// uv�l
};
