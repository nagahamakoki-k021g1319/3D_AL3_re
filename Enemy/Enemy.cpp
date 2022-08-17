#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//初期座標をセット
	worldTransform_.translation_ = {20, 10, 10};

	//ワールド変換の初期化
	worldTransform_.Initialize();

	/*Fire();*/
	Approach();
}

void Enemy::Update() {

	//デスフラグの立った弾の削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->InDead(); });

	//敵の移動ベクトル
	Vector3 move = {0, 0, 0};
	//敵の移動の速さ
	const float kCharacterSpeed = 0.1f;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	//移動(ベクトルを加算)
	worldTransform_.translation_ += {0, 0, -kCharacterSpeed};

	//弾更新
	//複数
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//発射タイマーカウントダウン
	shotTimer--;

	if (shotTimer == 0) {
		//弾発射
		Fire();
		//発射タイマー初期化
		shotTimer = kFireInterval;
	}



	debugText_->SetPos(50, 180);
	debugText_->Printf(
	  "Enemytranslation : %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//弾更新
	//複数
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Enemy::Fire() {

	//弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//弾を生成し初期化
	//複数
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();

	//単発
	/*PlayerBullet* newBullet = new PlayerBullet();*/
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾の登録
	//複数
	bullets_.push_back(std::move(newBullet));
}

void Enemy::Approach() { 
	//発射タイマーを初期化
	shotTimer = kFireInterval;
}
