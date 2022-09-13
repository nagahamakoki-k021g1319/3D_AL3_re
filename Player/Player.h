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
	void Initialize(Model* jikiNormal,Model* model, uint32_t textureHandle);

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
	Model* modelNormal_ = nullptr;
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
	int isPlayerChange = 1;
	int oldPlayerChangeMode = 0;

	const float kSpeedParamVel = 1.0f;
	bool isModeChangeBoost = false;
	int boostCount = 0;
	float turnAngle = 0.0f; //変形児の旋回
	float boostVelX = 0.0f;
	float boostVelZ = 0.0f;

	bool isPushTrans = false;

	//アングル用
	float primaryAngle = 0.0f;
	float controlAngleX = 0.0f;
	float controlAngleY = 0.0f;
	float angleVelocity = 0.0f;

	//通常携帯時のみで使う↓
	//入力時のhogehoge
	int isInitAngleMode = 0;
	
	//スピードの可変値
	Vector3 speedUpParam = {0.0f,0.0f,0.0f};
	
	//飛んでいるか否か
	float gravityVel = 0.0f;
	bool isFly = 0;
};
