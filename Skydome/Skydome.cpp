#include "Skydome.h"

void Skydome::Initialize(Model* model) { 
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(100, 100, 100);
	worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection viewProjection_) {
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection_);
}
