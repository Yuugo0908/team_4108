#pragma once

#include "Operator.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Object3d.h"
#include "CsvFile.h"

//�T�C�Y(��{��1)
const int csv_size = 1;

class CsvFile
{
public:
	// �f�B���N�g��
	static const std::string baseDirectory;
	// �g���q
	static const std::string Extension;
	// ���݂̏c��
	static int now_x, now_y;
	// �}�b�v�ύX�t���O
	static bool map_change_flag;
	// �`�F�b�N�|�C���g�̏c��
	static int check_x, check_y;
	// �`�F�b�N�|�C���g�̍��W
	static XMFLOAT3 check_pos;
	// �`�F�b�N�|�C���g�ύX�t���O
	static bool check_change_flag;
	// �c���̍ő吔
	static const int max_x, max_y;
	// �����}�b�v�̃t���O
	static bool init_flag;
public:
	static void CsvToVector(std::vector<std::vector<int>>& csvData, const std::string& fileName);

	static void SetInitNum(std::vector<int>& csv);
};