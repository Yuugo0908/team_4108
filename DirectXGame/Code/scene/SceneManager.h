#pragma once
#include "BaseScene.h"
#include "AbstructSceneFactory.h"

class SceneManager
{
public:
	// デストラクタ
	~SceneManager();

	static SceneManager* GetInstance();
	// 更新
	void Update();
	//描画
	void Draw();
	// シーンファクトリーのセッター
	void SetSceneFactory(AbstructSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
	// 次のシーンを予約
	void ChangeScene(const std::string& sceneName);

private:
	// 今のシーン(実行中のシーン)
	BaseScene* scene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;
	// 借りてくるシーンファクトリー
	AbstructSceneFactory* sceneFactory_ = nullptr;
};

