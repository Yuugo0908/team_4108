#pragma once

#include "Image2d.h"
#include <Windows.h>
#include <string>

class DebugText
{
public:
	// デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 256; // 最大文字数
	static const int fontWidth = 9; // フォント画像内1文字分の横幅
	static const int fontHeight = 18; // フォント画像内1文字分の縦幅
	static const int fontLineCount = 14; // フォント画像内1行分の文字数

	static DebugText* GetInstance();

	void Initialize(UINT texnumber);

	void Print(float x, float y, float scale, std::string text, ...);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:
	// 画像データの配列
	Image2d* image2dDatas[maxCharCount] = {};
	// 画像データ配列の添え字番号
	int image2dIndex = 0;
};