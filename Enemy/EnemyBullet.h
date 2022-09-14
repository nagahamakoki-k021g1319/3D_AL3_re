#pragma once
#include "Vector3.h"
#include <Model.h>
#include "ViewProjection.h"
#include <Model.h>
#include <WorldTransform.h>
#include <cassert>
#include <affin.h>

class Player;
class EnemyBullet {
public:
	///< summary>
	///初期化
	///</summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	///< summary>
	///初期化
	///</summary>
	void Update();

	///< summary>
	///初期化
	///</summary>
	void Draw(const ViewProjection& viewProjection);

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

private:


	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デス大麻
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	Player* player_;

	//誘導タイマー
	int inductionTimer = 120;
};
