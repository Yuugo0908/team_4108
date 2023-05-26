#pragma once

#include "Operator.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
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
	// マップ変更フラグ
	static bool map_change_flag;
	// チェックポイントの縦横
	static int check_x, check_y;
	// チェックポイントの座標
	static XMFLOAT3 check_pos;
	// チェックポイント変更フラグ
	static bool check_change_flag;
	// 縦横の最大数
	static const int max_x, max_y;
	// 初期マップのフラグ
	static bool init_flag;
public:
	static void CsvToVector(std::vector<std::vector<int>>& csvData, const std::string& fileName);

	static void SetInitNum(std::vector<int>& csv);
};