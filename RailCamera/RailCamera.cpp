#include "RailCamera.h"

void RailCamera::Initialize(const Vector3 position, const Vector3 rota) {

	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rota;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	

	////キャラクターの移動ベクトル
	//Vector3 move = {0, 0, 0};
	//Vector3 rotate = {0, 0, 0};
	////キャラクターの移動の速さ
	//const float kCharacterSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_UP)) {
	//	move = {0, kCharacterSpeed, 0};
	//} else if (input_->PushKey(DIK_DOWN)) {
	//	move = {0, -kCharacterSpeed, 0};
	//}
	//if (input_->PushKey(DIK_LEFT)) {
	//	move = {-kCharacterSpeed, 0, 0};
	//} else if (input_->PushKey(DIK_RIGHT)) {
	//	move = {kCharacterSpeed, 0, 0};
	//}

	//worldTransform_.translation_ += move;

	////回転
	//const float kChestRotSpeed = 0.02f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_U)) {
	//	rotate = {0, kChestRotSpeed, 0};
	//} else if (input_->PushKey(DIK_I)) {
	//	rotate = {0, -kChestRotSpeed, 0};
	//}
	//worldTransform_.rotation_ += rotate;

	////行列更新
	//AffinTrans::affin(worldTransform_);

	//worldTransform_.TransferMatrix();

	//viewProjection_.eye = worldTransform_.translation_;
	////ワールド前方ベクトル
	//Vector3 forward(0, 0, 1);
	////レールカメラの回転を反映
	//forward = AffinTrans::MatVector(forward, worldTransform_.matWorld_);
	//////視点から前方に適当な距離進んだ位置が注視点
	//viewProjection_.target.x = viewProjection_.eye.x + forward.x;
	//viewProjection_.target.y = viewProjection_.eye.y + forward.y;
	//viewProjection_.target.z = viewProjection_.eye.z + forward.z;
	
	

	

	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映
	viewProjection_.up = AffinTrans::MatVector(up, worldTransform_.matWorld_);
	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();


}

ViewProjection& RailCamera::GetViewProjection() { 
	return viewProjection_; 
}

WorldTransform* RailCamera::GetWorldPosition() { 
    

	return &worldTransform_;
}

void RailCamera::SetWorldPos(Vector3 vec)
{
	worldTransform_.translation_ += vec;
	
}

