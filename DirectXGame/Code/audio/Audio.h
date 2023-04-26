#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>

class Audio
{// オーディオ
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // サブクラス
	// チャンクヘッダ
	struct Chunk
	{
		char id[4]; // チャンク毎のID
		int	size; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		Chunk chunk;   // "RIFF"
		char type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		Chunk chunk; // "fmt "
		WAVEFORMAT fmt; // 波形フォーマット
	};

	static Audio* GetInstance();

public: // メンバ関数
	// 初期化
	bool Initialize();

	// サウンドファイルの読み込みと再生
	void PlayWave(const char* filename, int loopCount, float volume);

	void Stop();
private: // メンバ変数
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice *masterVoice = nullptr;
	IXAudio2SourceVoice *pSourceVoice = nullptr;
};