#include "EnemyReticle.h"

void EnemyReticle::Initialize(Model* model, uint32_t textureHandle)
{
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1, 1, 1 };
}

void EnemyReticle::Update()
{
}

void EnemyReticle::Draw(ViewProjection viewProjection_)
{
}
