#include "EnemyBullet.h"
#include "Player.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("eb.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	

}

void EnemyBullet::Update() {

	if (inductionTimer > 0) {
		inductionTimer--;
		//敵弾から自キャラへのベクトルを計算
		Vector3 toPlayer =
		{
		player_->GetWorldPosition2().x - worldTransform_.translation_.x,
		player_->GetWorldPosition2().y - worldTransform_.translation_.y,
		player_->GetWorldPosition2().z - worldTransform_.translation_.z
		};
		//ベクトルを正規化する
		toPlayer.normalize();
		velocity_.normalize();
		//球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
		velocity_ = velocity_.SphereLinear(velocity_, velocity_, toPlayer, 0.5f);
		velocity_ = { velocity_.x * 0.1f,velocity_.y * 0.1f ,velocity_.z * 0.1f };
	}
		//進行方向に見た目の回転を合わせる
		worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
		Vector3 temp = velocity_;
		temp.y = 0.0f;
		worldTransform_.rotation_.x = std::atan2(-velocity_.y, temp.length());
	
	//座標を移動
	worldTransform_.translation_ += velocity_;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() { 
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::OnCollision() {

	//デスフラグ
	isDead_ = true;
}
