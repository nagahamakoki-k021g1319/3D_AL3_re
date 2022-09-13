#include "EnemyBullet.h"
#include "Player.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("eb.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	

}

void EnemyBullet::Update() {

	if (inductionTimer > 0) {
		inductionTimer--;
		//�G�e���玩�L�����ւ̃x�N�g�����v�Z
		Vector3 toPlayer =
		{
		player_->GetWorldPosition2().x - worldTransform_.translation_.x,
		player_->GetWorldPosition2().y - worldTransform_.translation_.y,
		player_->GetWorldPosition2().z - worldTransform_.translation_.z
		};
		//�x�N�g���𐳋K������
		toPlayer.normalize();
		velocity_.normalize();
		//���ʐ��`��Ԃɂ��A���̑��x�Ǝ��L�����ւ̃x�N�g������}���A�V���ȑ��x�Ƃ���
		velocity_ = velocity_.SphereLinear(velocity_, velocity_, toPlayer, 0.5f);
		velocity_ = { velocity_.x * 0.1f,velocity_.y * 0.1f ,velocity_.z * 0.1f };
	}
		//�i�s�����Ɍ����ڂ̉�]�����킹��
		worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
		Vector3 temp = velocity_;
		temp.y = 0.0f;
		worldTransform_.rotation_.x = std::atan2(-velocity_.y, temp.length());
	
	//���W���ړ�
	worldTransform_.translation_ += velocity_;

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() { 
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ�����
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::OnCollision() {

	//�f�X�t���O
	isDead_ = true;
}
