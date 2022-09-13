#include "EnemyReticle.h"

void EnemyReticle::Initialize(Model* model, uint32_t textureHandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
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
