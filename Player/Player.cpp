#include "Player.h"


void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	
	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

}

void Player::Update() {
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	worldTransform_.translation_ += move;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "translation : %f,%f,%f", worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection_) { 
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

}


