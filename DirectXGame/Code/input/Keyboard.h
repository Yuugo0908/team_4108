#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "WinApp.h"

class Keyboard
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	Keyboard();

	~Keyboard();
public:
	// �R�s�[�R���X�g���N�^�𖳌���
	Keyboard(const Keyboard& obj) = delete;
	// ������Z�q�𖳌���
	Keyboard& operator=(const Keyboard& obj) = delete;

public: //�����o�֐�

	static Keyboard* GetInstance();
	//������
	bool Initialize(WinApp* win);
	//�X�V
	void Update();
	// �L�[�̉������`�F�b�N
	bool PushKey(BYTE keyNumber);
	// �L�[�̃g���K�[���`�F�b�N
	bool TriggerKey(BYTE keyNumber);

private: //�����o�ϐ�
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
};