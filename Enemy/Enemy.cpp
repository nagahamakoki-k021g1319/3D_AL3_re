#include "Enemy.h"
#include "Player.h"
#include <GameScene.h>
#include <cmath>

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 vector3) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	effectModel_ = Model::CreateFromOBJ("explosion", false);
	bulletModel_ = Model::CreateFromOBJ("tamaX", false);
	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�������W���Z�b�g
	worldTransform_.translation_ = vector3;

	/*Fire();*/
	Approach();
}

void Enemy::Update() {

	//�G�̈ړ��̑���
	const float kCharacterSpeed = 0.1f;

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += {0, 0, -kCharacterSpeed};
	
	//���˃^�C�}�[�J�E���g�_�E��
	shotTimer--;

	if (shotTimer <= 0) {
		//�e����
		Fire();
		//���˃^�C�}�[������
		shotTimer = kFireInterval;
	}


	debugText_->SetPos(10, 50);
	debugText_->Printf("%d", enemyId);
}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	
}

void Enemy::Fire() {

	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = 2.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//�v���C���[�̃��[���h���W�̎擾
	Vector3 playerPosition;
	playerPosition = player_->GetWorldPosition2();
	//�G�̃��[���h���W���擾
	Vector3 enemyPosition;
	enemyPosition = GetWorldPosition();
	//�����x�N�g�������߂�
	Vector3 A_BVec = Vector3(
	  playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y,
	  playerPosition.z - enemyPosition.z);
	//�x�N�g�����K��
	float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z) * 10;
	//�x�N�g���̒����𑬂��ɍ��킹��
	A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);
	A_BVec*= kBulletSpeed;

	//�e�𐶐���������
	//����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();

	newBullet->SetPlayer(player_);

	//�P��
	/*PlayerBullet* newBullet = new PlayerBullet();*/
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, A_BVec);
	
	//�e��o�^����
	gameScene_->AddEnemyBullet(newBullet);
	
}

void Enemy::Approach() { 
	//���˃^�C�}�[��������
	shotTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() { 
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ�����
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() { 
	for (int i = 0; i < 10; i++) {
		std::unique_ptr<Effect> newEffect = std::make_unique<Effect>();
		newEffect->Initialize(effectModel_, worldTransform_.translation_);
		gameScene_->AddEffect(newEffect);
	}
	isDead_ = true; 
}
