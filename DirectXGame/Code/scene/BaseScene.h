#pragma once
class BaseScene
{
public:
	virtual ~BaseScene() = default;

	// ������
	virtual void Initialize() = 0;

	// �I��
	virtual void Finalize() = 0;

	// ���t���[���X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;
};

