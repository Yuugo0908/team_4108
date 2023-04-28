#pragma once
#include <random>

/// <summary>
/// ����
/// </summary>
class Random
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// �����擾
	/// </summary>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>�����l</returns>
	inline static int GetRanNum(int min, int max)
	{
		// �����G���W���̃V�[�h�l
		std::random_device randev;
		// �����Z���k�E�c�C�X�^�[
		std::mt19937 eng(randev());
		// �͈�
		std::uniform_int_distribution<int> dist(min, max);

		return dist(eng);
	}
};