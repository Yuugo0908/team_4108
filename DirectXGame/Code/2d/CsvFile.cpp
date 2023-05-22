#include "CsvFile.h"

const std::string CsvFile::baseDirectory = "Resources/csv/";
const std::string CsvFile::Extension = ".csv";

// ���݂̏c��
int CsvFile::now_x = 0;
int CsvFile::now_y = 0;
//�c���̍ő吔
const int CsvFile::max_x = 10;
const int CsvFile::max_y = 10;
// �������t���O
bool CsvFile::nowInitFlag = false;

void CsvFile::CsvToVector(std::vector<std::vector<int>>& csvData, const std::string& fileName)
{
	std::ifstream ifs(baseDirectory + fileName + Extension);//�t�@�C���l�[���̊m�F
	std::string line; // 1�s�̕�����
	std::vector<int> csv;

	// CSV�t�@�C���̍Ō�̍s�܂Ń��[�v����
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string field;
		std::vector<int> result;

		while (std::getline(stream, field, ','))//�J���}��؂��
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

		// �s�I���܂�
		csvData.push_back(csv);

		// ���s
		for (auto it = csv.begin(); it != csv.end();)
		{
			it = csv.erase(it);
		}
	}

	// �ǂݍ��݂��I�������t���O��߂�
	nowInitFlag = false;
}

void CsvFile::SetInitNum(std::vector<int>& csv)
{
	const int initNum = 0;
	// �s�̒�����0��T��(�����}�b�v����)
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
