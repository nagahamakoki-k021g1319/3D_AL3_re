#include "Enemy.h"
#include "Player/Player.h"

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

	//敵の移動の速さ
	const float kCharacterSpeed = 0.1f;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	//移動(ベクトルを加算)
	worldTransform_.translation_ += {0, 0, -kCharacterSpeed};

	//発射タイマーカウントダウン
	shotTimer--;

	if (shotTimer == 0) {
		//弾発射
		Fire();
		//発射タイマー初期化
		shotTimer = kFireInterval;
	}

	//弾更新
	//複数
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
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

	/*assert(player_);*/

	//弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	////プレイヤーのワールド座標の取得
	//Vector3 playerPosition;
	//playerPosition = player_->GetWorldPosition2();
	////敵のワールド座標を取得
	//Vector3 enemyPosition;
	//enemyPosition = GetWorldPosition();
	////差分ベクトルを求める
	//Vector3 A_BVec = Vector3(
	//  playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y,
	//  playerPosition.z - enemyPosition.z);
	////ベクトル正規化
	//float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z) * 10;
	////ベクトルの長さを速さに合わせる
	//A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);


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

//Vector3 Enemy::GetWorldPosition() { 
//	//ワールド座標を入れる変数
//	Vector3 worldPos;
//	//ワールド行列の平行移動成分
//	worldPos.x = worldTransform_.translation_.x;
//	worldPos.y = worldTransform_.translation_.y;
//	worldPos.z = worldTransform_.translation_.z;
//
//	return worldPos;
//}
