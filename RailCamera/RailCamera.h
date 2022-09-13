#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin.h"
#include "Input.h"
#include "DebugText.h"

///< summary>
///レールカメラ
///</summary>
class RailCamera {
  public:
	///< summary>
	///初期化
	///</summary>
	void Initialize(const Vector3 position, const Vector3 rota);

	///< summary>
	///更新
	///</summary>
	void Update();

	ViewProjection& GetViewProjection();

	//ワールド座標を取得
	WorldTransform* GetWorldPosition();

	//セッター
	void SetWorldPos(Vector3 vec);

	void ResetRailCamera();

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	WorldTransform resetWorldTransform;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

};
