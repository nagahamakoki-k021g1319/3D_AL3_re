#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Enemy::Update() {
	//敵の移動ベクトル
	Vector3 move = {0, 0, 0};
	//敵の移動の速さ
	const float kCharacterSpeed = 0.2f;
	//敵の移動処理
	move = {0, 0, -kCharacterSpeed};
	worldTransform_.translation_ += move;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}