#include "Player.h"

using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	
	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//3D���e�B�N���p���[���h�g�����X�t�H�[��������
	worldTransform3DReticle_.Initialize();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 40};

	//���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle = TextureManager::Load("tage.png");
	//�X�v���C�g����
	sprite2DReticle_.reset(
	  Sprite::Create(textureReticle, Vector2(500, 350), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));


}

void Player::Update(ViewProjection viewProjection_) {
	
	//�f�X�t���O�̗������e�̍폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { 
		return bullet->IsDead();  
	});

	
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_W)) {
		move = {0, kCharacterSpeed, 0};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, -kCharacterSpeed, 0};
	}
	if (input_->PushKey(DIK_A)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_D)) {
		move = {kCharacterSpeed, 0, 0};
	}

	worldTransform_.translation_ += move;

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

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
	if (input_->PushKey(DIK_J)) {
		worldTransform_.rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_K)) {
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

	//---------���@�̃��[���h���W����3D���e�B�N���̃��[���h���W���v�Z-----------//
	
	//���@����3D���e�B�N���ւ̋���
	const float kDistancePlayerTo3DReticle = 60.0f;
	//���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
	Vector3 offset = {0, 0, 1.0f};
	//���@�̃��[���h���W�̉�]�𔽉f
	offset = AffinTrans::MatVector(offset,worldTransform_.matWorld_);
	//�x�N�g���̒����𐮂���
	offset = Vector3Normalize(offset) * kDistancePlayerTo3DReticle;
	//3D���e�B�N�����W�ݒ�
	worldTransform3DReticle_.translation_ =
	  offset + Vector3(
		  worldTransform_.matWorld_.m[3][0], 
		  worldTransform_.matWorld_.m[3][1],
	      worldTransform_.matWorld_.m[3][2]
	  );
	//�s��X�V
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();
	
	/////////////////////////////////////////////////////////////////


	//----------3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z-------//

	Vector3 positionReticle = AffinTrans::GetWorldtransform(worldTransform3DReticle_.matWorld_);

	Vector2 windowWH =
	  Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	//�r���[�|�[�g�s��
	Matrix4 Viewport = {
	   windowWH.x / 2,				  0,  0,  0, 
					0,	-windowWH.y / 2,  0,  0, 
				    0,				  0,  1,  0, 
	   windowWH.x / 2,	 windowWH.y / 2,  0,  1
	};
	  
	//�r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
	Matrix4 matViewProjectionViewport = viewProjection_.matView * viewProjection_.matProjection * Viewport;

	//���[���h���X�N���[�����W�ϊ�(��������3D����2D�ɂȂ�)
	positionReticle = AffinTrans::wDivision(positionReticle, matViewProjectionViewport);

	//�X�v���C�g�̃��e�B�N���ɍ��W�ݒ�
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	////////////////////////////////////////////////////////////////////////////////

	//----------�X�N���[�����W����2D���e�B�N���̃��[���h���W���v�Z-------//

	POINT mousePosition;
	//�}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	//�r���[�v���W�F�N�V�����r���[�|�[�g����
	Matrix4 matVPV = viewProjection_.matView * viewProjection_.matProjection * Viewport;

	//�����s��̋t�s����v�Z����
	Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	//�X�N���[�����W
	Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

	//�X�N���[�����W�n���烏�[���h���W�n��
	posNear = AffinTrans::wDivision(posNear, matInverseVPV);
	posFar = AffinTrans::wDivision(posFar, matInverseVPV);

	//�}�E�X���C�̕���
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Vector3Normalize(mouseDirection);
	//�J��������Ə��I�u�W�F�N�g�̋���
	const float kDistanceTestObject = 222.0f;
	worldTransform3DReticle_.translation_ = AffinTrans::AddVector(posNear, mouseDirection * kDistanceTestObject);

	//�s��X�V
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

	/////////////////////////////////////////////////////////////

	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "translation : %f,%f,%f", worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	DebugText::GetInstance()->SetPos(20, 200);
	DebugText::GetInstance()->Printf("Mouse ScreenPos:(%d,%d)", mousePosition.x, mousePosition.y);
	DebugText::GetInstance()->SetPos(20, 220);
	DebugText::GetInstance()->Printf("Near:(%f,%f,%f)", posNear.x, posNear.y, posNear.z);
	DebugText::GetInstance()->SetPos(20, 240);
	DebugText::GetInstance()->Printf("Far:(%f,%f,%f)", posFar.x, posFar.y, posFar.z);
	DebugText::GetInstance()->SetPos(20, 260);
	DebugText::GetInstance()->Printf(
	  "MouseObject:(%f,%f,%f)", worldTransform3DReticle_.translation_.x,
	  worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);


}

void Player::Draw(ViewProjection viewProjection_) { 
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	
	//�e�X�V
	//����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	//3D���e�B�N����`��
	model_->Draw(worldTransform3DReticle_, viewProjection_,textureHandle_);


	//�P��
	/*if (bullet_) {
		bullet_->Draw(viewProjection_);
	}*/
}

void Player::Attack() { 
	if (input_->IsTriggerMouse(0)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//���@����W���I�u�W�F�N�g�ւ̃x�N�g��
		velocity = AffinTrans::GetWorldtransform(worldTransform3DReticle_.matWorld_) - AffinTrans::GetWorldtransform(worldTransform_.matWorld_);
		velocity = Vector3Normalize(velocity) * kBulletSpeed;

		//�e�𐶐���������
		//����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		//�P��
		/*PlayerBullet* newBullet = new PlayerBullet();*/
		newBullet->Initialize(model_, AffinTrans::GetWorldtransform(worldTransform_.matWorld_), velocity);

		 //�e�̓o�^
		//����
		bullets_.push_back(std::move(newBullet));

		//�P��
		/*bullet_.reset(newBullet);*/
	}

}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();

}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) { 
	
	Vector3 result = {0, 0, 0};

	
	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
	           velocity.y * worldTransform.matWorld_.m[1][0] +
	           velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
	           velocity.y * worldTransform.matWorld_.m[1][1] +
	           velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
	           velocity.y * worldTransform.matWorld_.m[1][2] +
	           velocity.z * worldTransform.matWorld_.m[2][2];


	return result;
}

Vector3 Player::GetWorldPosition2() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ�����
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos; 
}

void Player::OnCollision() {}

void Player::setparent(WorldTransform* worldTransform) { 
	worldTransform_.parent_ = worldTransform; 
}


