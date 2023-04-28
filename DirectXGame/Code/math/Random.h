#pragma once
#include <random>

/// <summary>
/// 乱数
/// </summary>
class Random
{
public: // 静的メンバ関数
	/// <summary>
	/// 乱数取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>乱数値</returns>
	inline static int GetRanNum(int min, int max)
	{
		// 乱数エンジンのシード値
		std::random_device randev;
		// メルセンヌ・ツイスター
		std::mt19937 eng(randev());
		// 範囲
		std::uniform_int_distribution<int> dist(min, max);

		return dist(eng);
	}
};