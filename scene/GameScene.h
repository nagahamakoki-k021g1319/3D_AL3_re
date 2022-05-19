#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <math.h>

const float π = 3.14;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	///////////////////////////////////////////////
	//------------初期ボックスの変数や処理------------//
	///////////////////////////////////////////////

	 Vector3 vector3[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
      {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};

	Vector3 vector3Box[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
      {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};

	int eqgeList[12][2] = {
	  {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
      {0, 4},
      {1, 5},
	  {2, 6},
      {3, 7},
      {4, 5},
      {5, 6},
      {6, 7},
      {7, 4}
    };

	Vector4 move[4] = {
	  {1.0f, 0.0f, 0.0f, 10.0f},
	  {0.0f, 1.0f, 0.0f, 10.0f},
	  {0.0f, 0.0f, 1.0f, 10.0f},
	  {0,    0,    0,    1    }
    };

	Vector3 vector3f;
	Vector4 vector4;


	///////////////////////////////////////////////
	//------------拡大ボックスの変数や処理------------//
	///////////////////////////////////////////////

	//拡大用のボックス
	 Vector3 vector3a[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};
	//拡大させたボックス
	Vector3 vector3aBox[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};

	//拡大するための関数
	Vector4 Scale[4]{
	  {2.0f, 0.0f, 0.0f, 0.0f},
	  {0.0f, 2.0f, 0.0f, 0.0f},
	  {0.0f, 0.0f, 2.0f, 0.0f},
	  {0.0f, 0.0f, 0.0f, 1.0f}
    };
	

	///////////////////////////////////////////////
	//------------回転ボックスの変数や処理------------//
	///////////////////////////////////////////////

	//回転用のボックス
	Vector3 vector3b[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};
	//回転させたボックス
	Vector3 vector3bBox[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};
	//回転させたXボックス
	Vector3 vector3bXBox[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};
	//回転させたYボックス
	Vector3 vector3bYBox[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};
	//回転させたZボックス
	Vector3 vector3bZBox[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};
	//回転するためのX軸関数
	Vector4 rotateX[4]{
	  {1.0f,  0.0f,       0.0f,       0.0f},
	  {0.0f,  cos(π / 4), sin(π / 4), 0.0f},
	  {0.0f, -sin(π / 4), cos(π / 4), 0.0f},
	  {0.0f,  0.0f,       0.0f,       1.0f}
    };
	//回転するためのY軸関数
	Vector4 rotateY[4]{
	  {cos(π / 4), 0.0f, -sin(π / 4),  0.0f},
	  {0.0f,       1.0f,  0.0f,        0.0f},
	  {sin(π / 4), 0.0f,  cos(π / 4),  0.0f},
	  {0.0f,       0.0f,  0.0f,        1.0f}
    };
	//回転するためのZ軸関数
	Vector4 rotateZ[4]{
	  { cos(π / 4), sin(π / 4), 0.0f, 0.0f},
	  {-sin(π / 4), cos(π / 4), 0.0f, 0.0f},
	  { 0.0f,       0.0f,       1.0f, 0.0f},
	  { 0.0f,       0.0f,       0.0f, 1.0f}
    };

	
	///////////////////////////////////////////////
	//-----------平行移動ボックスの変数や処理----------//
	///////////////////////////////////////////////

	//平行移動用のボックス
	Vector3 vector3c[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};
	//平行移動させたボックス
	Vector3 vector3cBox[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
	};
	//平行移動の関数
	Vector4 parallelMove[4] = {
	  {1.0f, 0.0f, 0.0f, 5.0f},
	  {0.0f, 1.0f, 0.0f, 5.0f},
	  {0.0f, 0.0f, 1.0f, 5.0f},
	  {0.0f, 0.0f, 0.0f, 1.0f}
    };


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
