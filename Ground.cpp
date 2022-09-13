#include "Ground.h"

void Ground::Initialize(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = Vector3(0, -0.05f, 0);
	worldTransform_.scale_ = Vector3(600, 100, 600);
	worldTransform_.matWorld_ *= AffinTrans::Move(worldTransform_.translation_);
	worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Ground::Update() {}

void Ground::Draw(ViewProjection viewProjection_) {
	// 3D���f���`��
	model_->Draw(worldTransform_, viewProjection_);
}
