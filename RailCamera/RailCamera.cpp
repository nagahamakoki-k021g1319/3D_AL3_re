#include "RailCamera.h"

void RailCamera::Initialize(const Vector3 position, const Vector3 rota) {

	//ShowCursor(FALSE);

	viewProjection_.farZ = 2500;


	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rota;

	resetWorldTransform.translation_ = position;
	resetWorldTransform.rotation_ = rota;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() {
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

void RailCamera::SetWorldPos(Vector3 vec){
	worldTransform_.translation_ += vec;
}

void RailCamera::ResetRailCamera()
{
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.rotation_ = { 0,0,0 };
	viewProjection_.eye = { 0,0,-50.0f };

	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();
	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

