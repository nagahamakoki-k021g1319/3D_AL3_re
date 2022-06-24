#include "Player.h"


void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	
	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update() {
	
	//�f�X�t���O�̗������e�̍폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { 
		return bullet->InDead();  
	});

	
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_UP)) {
		move = {0, kCharacterSpeed, 0};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -kCharacterSpeed, 0};
	}
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	worldTransform_.translation_ += move;

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	const float kChestRotSpeed = 0.05f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += kChestRotSpeed;
	}

	//�e���ˏ���
	Attack();

	//�e�X�V
	//����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�P��
	/*if (bullet_) {
		bullet_->Update();
	}*/

	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "translation : %f,%f,%f", worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection_) { 
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//�e�X�V
	//����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	//�P��
	/*if (bullet_) {
		bullet_->Draw(viewProjection_);
	}*/
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//�e�𐶐���������
		//����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		//�P��
		/*PlayerBullet* newBullet = new PlayerBullet();*/
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		 //�e�̓o�^
		//����
		bullets_.push_back(std::move(newBullet));

		//�P��
		/*bullet_.reset(newBullet);*/
	} 

}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) { 
	
	Vector3 result = {0, 0, 0};

	//����
	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
	           velocity.y * worldTransform.matWorld_.m[1][2] +
	           velocity.z * worldTransform.matWorld_.m[2][2];

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
	           velocity.y * worldTransform.matWorld_.m[1][0] +
	           velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
	           velocity.y * worldTransform.matWorld_.m[1][1] +
	           velocity.z * worldTransform.matWorld_.m[2][1];

	return result;
}


