#include "CsvFile.h"

const std::string CsvFile::baseDirectory = "Resources/csv/";
const std::string CsvFile::Extension = ".csv";

// 現在の縦横
int CsvFile::now_x = 0;
int CsvFile::now_y = 0;
//縦横の最大数
const int CsvFile::max_x = 10;
const int CsvFile::max_y = 10;
// 初期化フラグ
bool CsvFile::nowInitFlag = false;

void CsvFile::CsvToVector(std::vector<std::vector<int>>& csvData, const std::string& fileName)
{
	std::ifstream ifs(baseDirectory + fileName + Extension);//ファイルネームの確認
	std::string line; // 1行の文字列
	std::vector<int> csv;

	// CSVファイルの最後の行までループを回す
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string field;
		std::vector<int> result;

		while (std::getline(stream, field, ','))//カンマ区切りで
		{
			result.push_back(stoi(field));
		}

		for (int i : result)
		{
			csv.push_back(i);
		}

		if (!nowInitFlag)
		{
			SetInitNum(csv);
		}

		// 行終わりまで
		csvData.push_back(csv);

		// 改行
		for (auto it = csv.begin(); it != csv.end();)
		{
			it = csv.erase(it);
		}
	}

	// 読み込みが終わったらフラグを戻す
	nowInitFlag = false;
}

void CsvFile::SetInitNum(std::vector<int>& csv)
{
	const int initNum = 0;
	// 行の中から0を探す(初期マップ決め)
	auto itr = std::find(csv.begin(), csv.end(), initNum);
	if (itr == csv.end())
	{
		now_y++;
	}
	else
	{
		const _int64 wanted_index = std::distance(csv.begin(), itr);
		now_x = (int)wanted_index;
		nowInitFlag = true;
	}
}
