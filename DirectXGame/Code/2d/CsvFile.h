#pragma once

#include "Operator.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <SafeDelete.h>
#include "Object3d.h"
#include "CsvFile.h"

//サイズ(基本は1)
const int csv_size = 1;

class CsvFile
{
public:
	// ディレクトリ
	static const std::string baseDirectory;
	// 拡張子
	static const std::string Extension;
	// 現在の縦横
	static int now_x, now_y;
	//縦横の最大数
	static const int max_x, max_y;

	static bool nowInitFlag;
public:
	static void CsvToVector(std::vector<std::vector<int>>& csvData, const std::string& fileName);

	static void SetInitNum(std::vector<int>& csv);
};

