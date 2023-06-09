#pragma once
#include "Operator.h"
#include "Collision.h"
#include <string>
#include <vector>
#include <json.hpp>

struct LevelData
{
	struct ObjectData
	{
		// ファイル名
		std::string fileName;
		// オブジェクトタイプ
		std::string objType;
		// 平行移動
		XMVECTOR trans;
		// 回転
		XMVECTOR rot;
		// スケール
		XMVECTOR scale;
		// コライダー中心点
		XMVECTOR center;
		// コライダー大きさ
		XMVECTOR size;
		// 移動先の座標
		XMVECTOR movePos;
	};
	// オブジェクト配列
	std::vector<ObjectData> objects;

	// モデルリスト
	Model* biteModel = nullptr;
	Model* wallModel = nullptr;
	Model* floorModel = nullptr;
	Model* checkPointModel = nullptr;
	Model* movator = nullptr;
	Model* acid = nullptr;
	Model* door = nullptr;
	Model* key = nullptr;
	Model* board1 = nullptr;
	Model* board2 = nullptr;
	Model* board3 = nullptr;
	Model* board4 = nullptr;

	std::map<std::string, Model*> models;
};

class LevelLoader
{
public:
	// ディレクトリ
	static const std::string baseDirectory;
	// 拡張子
	static const std::string Extension;
public:
	// ファイル読み込み
	static LevelData* LoadFile(const std::string& fileName);

	static void Recursive(nlohmann::json& object, LevelData* levelData);
};