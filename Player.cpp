#include "Player.h"
#include "Input.h"
#include "GameScene.h"
#include "DebugText.h"

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//キーでキャラクターの移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	//座標移動
	

	//行列更新


	//キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 150);

}

void Player::Draw() {
}
