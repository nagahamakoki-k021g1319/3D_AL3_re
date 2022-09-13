#include "Effect.h"

void Effect::Initialize(Model* model, const Vector3& position)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	/*textureHandle_ = TextureManager::Load("EnemyBullet.jpg");*/

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = { 10,10,10 };

	worldTransform_.translation_ = { worldTransform_.translation_.x + rand() % 6 - 3 ,worldTransform_.translation_.y + rand() % 6 - 3 ,worldTransform_.translation_.z + rand() % 6 - 3 };
}

void Effect::Update()
{
	/*worldTransform_.translation_ = worldTransform_.translation_;*/
	if (worldTransform_.scale_.x <= 0) {
		isDead_ = true;
	}

	worldTransform_.scale_ = { worldTransform_.scale_.x - becomeSmaller,worldTransform_.scale_.y - becomeSmaller,worldTransform_.scale_.z - becomeSmaller };

	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();


	////座標を移動させる(1フレーム文の移動量を足しこむ)
	//worldTransform_.translation_ += velocity_;

}

void Effect::Draw(const ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_);
}
