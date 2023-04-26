#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>

class Audio
{// �I�[�f�B�I
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // �T�u�N���X
	// �`�����N�w�b�_
	struct Chunk
	{
		char id[4]; // �`�����N����ID
		int	size; // �`�����N�T�C�Y
	};

	// RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		Chunk chunk;   // "RIFF"
		char type[4]; // "WAVE"
	};

	// FMT�`�����N
	struct FormatChunk
	{
		Chunk chunk; // "fmt "
		WAVEFORMAT fmt; // �g�`�t�H�[�}�b�g
	};

	static Audio* GetInstance();

public: // �����o�֐�
	// ������
	bool Initialize();

	// �T�E���h�t�@�C���̓ǂݍ��݂ƍĐ�
	void PlayWave(const char* filename, int loopCount, float volume);

	void Stop();
private: // �����o�ϐ�
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice *masterVoice = nullptr;
	IXAudio2SourceVoice *pSourceVoice = nullptr;
};