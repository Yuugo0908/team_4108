#pragma once

#include "Operator.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <SafeDelete.h>
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
	//�c���̍ő吔
	static const int max_x, max_y;

	static bool nowInitFlag;
public:
	static void CsvToVector(std::vector<std::vector<int>>& csvData, const std::string& fileName);

	static void SetInitNum(std::vector<int>& csv);
};

