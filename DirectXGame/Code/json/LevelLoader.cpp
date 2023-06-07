#include "LevelLoader.h"
#include <fstream>

using namespace std;

const std::string LevelLoader::baseDirectory = "Resources/level/";
const std::string LevelLoader::Extension = ".json";

LevelData* LevelLoader::LoadFile(const std::string& fileName)
{
	// �t�@�C���X�g���[��
	std::ifstream file;

	//.odj�t�@�C�����J��
	file.open(baseDirectory + fileName + Extension);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	nlohmann::json deserialized;

	// ��
	file >> deserialized;

	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	assert(name.compare("scene") == 0);

	// ���x���f�[�^�i�[�p�C���X�^���X���쐬
	LevelData* levelData = new LevelData();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		Recursive(object, levelData);
	}

	// ���f���̓ǂݍ���
	levelData->biteModel = levelData->biteModel->CreateFromObject("bite");
	levelData->wallModel = levelData->wallModel->CreateFromObject("wall");
	levelData->floorModel = levelData->floorModel->CreateFromObject("floor");
	levelData->checkPointModel = levelData->checkPointModel->CreateFromObject("checkPoint");
	levelData->movator = levelData->movator->CreateFromObject("movator");
	levelData->acid = levelData->acid->CreateFromObject("acid");
	levelData->door = levelData->door->CreateFromObject("door");
	levelData->key = levelData->key->CreateFromObject("key");
	// fileName�����ɁA�g�p���郂�f�������߂�
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

	// ��ʂ��擾
	std::string type = object["type"].get<std::string>();

	// MESH
	if (type.compare("MESH") == 0)
	{
		// �v�f�ǉ�
		levelData->objects.emplace_back(LevelData::ObjectData{});
		// ���ǉ������v�f�̎Q�Ƃ𓾂�
		LevelData::ObjectData& objectData = levelData->objects.back();

		if (object.contains("file_name"))
		{
			// �t�@�C����
			objectData.fileName = object["file_name"];
		}
		else
		{
			assert(0);
		}

		// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
		nlohmann::json& transform = object["transform"];
		// ���s�ړ�
		objectData.trans.m128_f32[0] = (float)transform["translation"][0];
		objectData.trans.m128_f32[1] = (float)transform["translation"][1];
		objectData.trans.m128_f32[2] = (float)transform["translation"][2];
		objectData.trans.m128_f32[3] = 1.0f;
		// ��]�p
		objectData.rot.m128_f32[0] = (float)transform["rotation"][0];
		objectData.rot.m128_f32[1] = (float)transform["rotation"][1];
		objectData.rot.m128_f32[2] = (float)transform["rotation"][2];
		objectData.rot.m128_f32[3] = 0.0f;
		// �X�P�[�����O
		objectData.scale.m128_f32[0] = (float)transform["scaling"][0];
		objectData.scale.m128_f32[1] = (float)transform["scaling"][1];
		objectData.scale.m128_f32[2] = (float)transform["scaling"][2];
		objectData.scale.m128_f32[3] = 0.0f;

		// �R���C�_�[�̃p�����[�^�ǂݍ���
		if (object.contains("collider"))
		{
			nlohmann::json& collider = object["collider"];
			// �R���C�_�[�̒��S�_
			objectData.center.m128_f32[0] = (float)collider["center"][0];
			objectData.center.m128_f32[1] = (float)collider["center"][1];
			objectData.center.m128_f32[2] = (float)collider["center"][2];
			objectData.center.m128_f32[3] = 0.0f;
			// �R���C�_�[�̑傫��
			objectData.size.m128_f32[0] = (float)collider["size"][0];
			objectData.size.m128_f32[1] = (float)collider["size"][1];
			objectData.size.m128_f32[2] = (float)collider["size"][2];
			objectData.size.m128_f32[3] = 0.0f;
		}

		// �I�u�W�F�N�g�^�C�v�̓ǂݍ���
		if (object.contains("obj_type"))
		{
			objectData.objType = object["obj_type"];
		}

		// �ړ���̍��W��ǂݍ���
		if (object.contains("move_pos"))
		{
			nlohmann::json& move_pos = object["move_pos"];
			// �ړ���̍��W
			objectData.movePos.m128_f32[0] = (float)move_pos["move_pos"][1];
			objectData.movePos.m128_f32[1] = (float)move_pos["move_pos"][2];
			objectData.movePos.m128_f32[2] = (float)move_pos["move_pos"][0];
			objectData.movePos.m128_f32[3] = 0.0f;
		}
	}
}
