#pragma once
#include "Framework.h"

// �Q�[���S��
class MyGame : public Framework
{
public: // �����o�֐�

	// ������
	void Initialize() override;
	// �I��
	void Finalize() override;
	// ���t���[���X�V
	void Update() override;
	// �`��
	void Draw() override;
};