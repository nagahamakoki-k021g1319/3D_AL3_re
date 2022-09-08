#pragma once
#include "Input.h"
#include <cassert>
#include "DebugText.h"
#include <Model.h>
#include <WorldTransform.h>
#include <affin.h>
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include <Vector3.h>
#include <WinApp.h>

class Player {
  public:
	///<summary>
	///初期化
	///</summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///初期化
	///</summary>
	void Update(ViewProjection viewProjection_);

	///< summary>
	///初期化
	///</summary>
	void Draw(ViewProjection viewProjection_);

	///< summary>
	///初期化
	///</summary>
	void Attack();

	///< summary>
	/// UI描画
	///</summary>
	void DrawUI();


	//キャラの向きに応じた方向に球をだす
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	//ワールド座標を取得
	Vector3 GetWorldPosition2();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void setparent(WorldTransform* worldTransform);



	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;


	//弾
	//複数 
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;


	// MSと変形機のチェンジ
	int isPlayerChange = 0;

	bool isBoost = false;
	bool isOldBoost = false;
	bool isPushTrans = false;
	bool isOldPush = false;
	float playerAngle = 0.0f;
	int quadrant = 0;
	

};
