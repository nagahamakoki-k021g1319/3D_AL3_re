#include "Enemy.h"
#include "Player.h"
#include <GameScene.h>
#include <cmath>

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 vector3) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	effectModel_ = Model::CreateFromOBJ("explosion", false);
	bulletModel_ = Model::CreateFromOBJ("tamaX", false);


	//シングルインスタンスを取得する

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//初期座標をセット
	worldTransform_.translation_ = vector3;
	worldTransform_.scale_ = { 3,3,3 };

	/*Fire();*/
	Approach();

	if (enemyId == 0) {
		moveCount = 0;
	}else if (enemyId == 1) {
		moveCount = 60;
	}
	else if (enemyId == 2) {
		moveCount = 120;
	}
	else if (enemyId == 3) {
		moveCount = 180;
	}


}

void Enemy::Update() {

	//敵の移動の速さ
	float kCharacterSpeed = 0.0f;

	float kCharacterSpeedX = 0.0f;
	float kCharacterSpeedX2 = 0.0f;
	//行列更新



	shotTimer--;

	if (shotTimer == 0) {
		//弾発射
		Fire();
		//発射タイマー初期化
		shotTimer = kFireInterval;
	}

	if (enemyId <= 3 && enemyId >= 0) {
		moveCount++;
		if (moveCount >= 360 && moveMode == 0) {
			moveMode = 1;
			moveCount = 0;
		}
		else if (moveCount >= 360 && moveMode == 1) {
			moveMode = 0;
			moveCount = 0;
		}

	}
	if (enemyId <= 3 && enemyId >= 0) {
		if (moveMode = 1) {
			worldTransform_.translation_.x += sinf(moveCount *  MathUtility::PI / 180) * 0.4f;
			worldTransform_.translation_.y += cosf(moveCount *  MathUtility::PI / 180) * 0.4f;
		}
		else if (moveMode = 0) {
			worldTransform_.translation_.x += sinf(moveCount * MathUtility::PI / 180) * 0.4f;	//何も変わらん
			worldTransform_.translation_.y += cosf(moveCount * MathUtility::PI / 180) * 0.4f;
		}
	}

	worldTransform_.translation_ += {0, 0, -kCharacterSpeed};
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);

}

void Enemy::Fire() {

	assert(player_);


	//弾の速度
	const float kBulletSpeed = 11.0f;

	Vector3 velocity(0, 0, kBulletSpeed);

	//プレイヤーのワールド座標の取得
	Vector3 playerPosition;
	playerPosition = player_->GetWorldPosition2();
	//敵のワールド座標を取得
	Vector3 enemyPosition;
	enemyPosition = GetWorldPosition();
	//差分ベクトルを求める
	Vector3 A_BVec = Vector3(
		playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y,
		playerPosition.z - enemyPosition.z);
	//ベクトル正規化
	float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z) * 10;
	//ベクトルの長さを速さに合わせる
	A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);


	A_BVec *= kBulletSpeed;



	//弾を生成し初期化
	//複数
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();


	newBullet->SetPlayer(player_);


	//単発

	/*PlayerBullet* newBullet = new PlayerBullet();*/
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, A_BVec);

	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);

}

void Enemy::Approach() {
	//発射タイマーを初期化
	shotTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分
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


