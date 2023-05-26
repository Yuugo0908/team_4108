#include "CsvFile.h"

const std::string CsvFile::baseDirectory = "Resources/csv/";
const std::string CsvFile::Extension = ".csv";

// ���݂̏c��
int CsvFile::now_x = 0;
int CsvFile::now_y = 0;
// �`�F�b�N�|�C���g�̏c��
int CsvFile::check_x = 0;
int CsvFile::check_y = 0;
// �c���̍ő吔
const int CsvFile::max_x = 10;
const int CsvFile::max_y = 10;
// �`�F�b�N�|�C���g���W
XMFLOAT3 CsvFile::check_pos = {};
// �}�b�v�ύX�t���O
bool CsvFile::map_change_flag = false;
// �`�F�b�N�|�C���g�ύX�t���O
bool CsvFile::check_change_flag = false;
// �������t���O
bool CsvFile::init_flag = false;

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

		if (!init_flag)
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

	// �`�F�b�N�|�C���g�̏����}�b�v
	check_x = now_x;
	check_y = now_y;
	// �ǂݍ��݂��I�������t���O��߂�
	init_flag = false;
}

void CsvFile::SetInitNum(std::vector<int>& csv)
{
	const int initNum = 0;
	// �s�̒�����0��T��(�����}�b�v����)
	auto itr = std::find(csv.begin(), csv.end(), initNum);
	if (itr == csv.end())
	{
		// ������Ύ��̍s��
		now_y++;
	}
	else
	{
		// 0�܂ł̋��������̃}�b�v�ԍ��ɑ��
		const _int64 wanted_index = std::distance(csv.begin(), itr);
		now_x = (int)wanted_index;
		// �����}�b�v�����܂�����t���O��true�ɂ���
		init_flag = true;
	}
}
