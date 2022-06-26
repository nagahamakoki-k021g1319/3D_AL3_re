#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//初期座標をセット
	worldTransform_.translation_ = {0,10,10};

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Enemy::Update() {
	//敵の移動ベクトル
	Vector3 move = {0, 0, 0};
	//敵の移動の速さ
	const float kCharacterSpeed = 0.1f;
	////敵の移動処理
	//move = {0, 0, -kCharacterSpeed};
	//worldTransform_.translation_ += move;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		//移動(ベクトルを加算)
		worldTransform_.translation_ += {0, 0, -kCharacterSpeed};
		//規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		//移動(ベクトルを加算)
		worldTransform_.translation_ += {-kCharacterSpeed, kCharacterSpeed, -kCharacterSpeed};
		break;
	}

	debugText_->SetPos(50, 180);
	debugText_->Printf(
	  "Enemytranslation : %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);



}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}