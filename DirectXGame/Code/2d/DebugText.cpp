#include "DebugText.h"

DebugText* DebugText::GetInstance()
{
	static DebugText instance;

	return &instance;
}

void DebugText::Initialize(UINT texnumber)
{
	// �S�Ẳ摜�f�[�^�ɂ���
	for (int i = 0; i < _countof(image2dDatas); i++)
	{
		// �摜�𐶐�����
		image2dDatas[i] = Image2d::Create(texnumber, { 0,0 });
	}
}

// 1������ǉ�
void DebugText::Print(float x, float y, float scale, std::string text, ...)
{
	bool parsentFlag = false;
	va_list args;
	va_start(args, text);
	text.push_back(' ');
	for (size_t i = 0; text[i] != '\0'; ++i)
	{
		printf("%c:", text[i]);
		if (text[i] == '%')
		{
			parsentFlag = true;
		}
		if (parsentFlag == true)
		{
			int temp;
			double tempF = NULL;
			switch (text[i])
			{
			case 'd':
				temp = va_arg(args, int);
				text.replace((size_t)i - 1, 2, std::to_string(temp));
				parsentFlag = false;
				break;
			case 'f':
				tempF = va_arg(args, double);
				tempF = round(tempF * 100);
				tempF = tempF / 100;
				text.replace((size_t)i - 1, 2, std::to_string(tempF));
				parsentFlag = false;
				break;
			default:
				break;
			}
		}
	}
	va_end(args);

	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (image2dIndex >= maxCharCount)
		{
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		image2dDatas[image2dIndex]->SetPosition({ x + fontWidth * scale * i, y });
		image2dDatas[image2dIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		image2dDatas[image2dIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		// �������P�i�߂�
		image2dIndex++;
	}
}

// �܂Ƃ߂ĕ`��
void DebugText::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	// �S�Ă̕����̉摜�ɂ���
	for (int i = 0; i < image2dIndex; i++)
	{
		// �摜�`��
		image2dDatas[i]->Draw();
	}

	image2dIndex = 0;
}