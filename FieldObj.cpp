#include "FieldObj.h"

void FieldObj::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();


	worldTransform_.translation_ = Vector3(0, -0.05f, 100);
	worldTransform_.scale_ = Vector3(3, 3, 3);
	worldTransform_.matWorld_ *= AffinTrans::Move(worldTransform_.translation_);
	worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	//行列の転送
	worldTransform_.TransferMatrix();
}

void FieldObj::Update() {}

void FieldObj::Draw(ViewProjection viewProjection_) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_);
}

void FieldObj::SetTransform(Vector3 v)
{
	worldTransform_.matWorld_.m[3][0] = v.x;
	worldTransform_.matWorld_.m[3][1] = v.y;
	worldTransform_.matWorld_.m[3][2] = v.z;
}
