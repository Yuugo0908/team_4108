#include "LevelLoader.h"
#include <fstream>

using namespace std;

const std::string LevelLoader::baseDirectory = "Resources/level/";
const std::string LevelLoader::Extension = ".json";

LevelData* LevelLoader::LoadFile(const std::string& fileName)
{
	// ファイルストリーム
	std::ifstream file;

	//.odjファイルを開く
	file.open(baseDirectory + fileName + Extension);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを作成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		Recursive(object, levelData);
	}

	// モデルの読み込み
	levelData->biteModel = levelData->biteModel->CreateFromObject("bite");
	levelData->wallModel = levelData->wallModel->CreateFromObject("wall");
	levelData->floorModel = levelData->floorModel->CreateFromObject("floor");
	levelData->checkPointModel = levelData->checkPointModel->CreateFromObject("checkPoint");
	levelData->movator = levelData->movator->CreateFromObject("movator");
	levelData->acid = levelData->acid->CreateFromObject("acid");
	levelData->door = levelData->door->CreateFromObject("door");
	levelData->key = levelData->key->CreateFromObject("key");
	// fileNameを元に、使用するモデルを決める
	levelData->models.insert(std::make_pair("bite", levelData->biteModel));
	levelData->models.insert(std::make_pair("wall", levelData->wallModel));
	levelData->models.insert(std::make_pair("floor", levelData->floorModel));
	levelData->models.insert(std::make_pair("checkPoint", levelData->checkPointModel));
	levelData->models.insert(std::make_pair("movator", levelData->movator));
	levelData->models.insert(std::make_pair("acid", levelData->acid));
	levelData->models.insert(std::make_pair("door", levelData->door));
	levelData->models.insert(std::make_pair("key", levelData->key));
	return levelData;
}

void LevelLoader::Recursive(nlohmann::json& object, LevelData* levelData)
{
	assert(object.contains("type"));

	// 種別を取得
	std::string type = object["type"].get<std::string>();

	// MESH
	if (type.compare("MESH") == 0)
	{
		// 要素追加
		levelData->objects.emplace_back(LevelData::ObjectData{});
		// 今追加した要素の参照を得る
		LevelData::ObjectData& objectData = levelData->objects.back();

		if (object.contains("file_name"))
		{
			// ファイル名
			objectData.fileName = object["file_name"];
		}
		else
		{
			assert(0);
		}

		// トランスフォームのパラメータ読み込み
		nlohmann::json& transform = object["transform"];
		// 平行移動
		objectData.trans.m128_f32[0] = (float)transform["translation"][0];
		objectData.trans.m128_f32[1] = (float)transform["translation"][1];
		objectData.trans.m128_f32[2] = (float)transform["translation"][2];
		objectData.trans.m128_f32[3] = 1.0f;
		// 回転角
		objectData.rot.m128_f32[0] = (float)transform["rotation"][0];
		objectData.rot.m128_f32[1] = (float)transform["rotation"][1];
		objectData.rot.m128_f32[2] = (float)transform["rotation"][2];
		objectData.rot.m128_f32[3] = 0.0f;
		// スケーリング
		objectData.scale.m128_f32[0] = (float)transform["scaling"][0];
		objectData.scale.m128_f32[1] = (float)transform["scaling"][1];
		objectData.scale.m128_f32[2] = (float)transform["scaling"][2];
		objectData.scale.m128_f32[3] = 0.0f;

		// コライダーのパラメータ読み込み
		if (object.contains("collider"))
		{
			nlohmann::json& collider = object["collider"];
			// コライダーの中心点
			objectData.center.m128_f32[0] = (float)collider["center"][0];
			objectData.center.m128_f32[1] = (float)collider["center"][1];
			objectData.center.m128_f32[2] = (float)collider["center"][2];
			objectData.center.m128_f32[3] = 0.0f;
			// コライダーの大きさ
			objectData.size.m128_f32[0] = (float)collider["size"][0];
			objectData.size.m128_f32[1] = (float)collider["size"][1];
			objectData.size.m128_f32[2] = (float)collider["size"][2];
			objectData.size.m128_f32[3] = 0.0f;
		}

		// オブジェクトタイプの読み込み
		if (object.contains("obj_type"))
		{
			objectData.objType = object["obj_type"];
		}

		// 移動先の座標を読み込み
		if (object.contains("move_pos"))
		{
			nlohmann::json& move_pos = object["move_pos"];
			// 移動先の座標
			objectData.movePos.m128_f32[0] = (float)move_pos["move_pos"][1];
			objectData.movePos.m128_f32[1] = (float)move_pos["move_pos"][2];
			objectData.movePos.m128_f32[2] = (float)move_pos["move_pos"][0];
			objectData.movePos.m128_f32[3] = 0.0f;
		}
	}
}
