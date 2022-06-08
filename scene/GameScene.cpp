#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "Vector3.h"
#include <cassert>
#include "math.h"
#include "affin.h"
#include <random>
#include "MathUtility.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイルを指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float value = dist(engine);

	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, MathUtility::PI2);


	for (WorldTransform& worldTransform : worldTransforms_) {

		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		// X,Y,Z 方向のスケーリングを設定
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
		// X,Y,Z 軸回りの回転角を設定
		worldTransform.rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		// X,Y,Z軸の周りの平行移動を設定
		worldTransform.translation_ = {dist(engine), dist(engine), dist(engine)};
		//合成の計算
		worldTransform.matWorld_.m[0][0] = 1.0f;
		worldTransform.matWorld_.m[1][1] = 1.0f;
		worldTransform.matWorld_.m[2][2] = 1.0f;
		worldTransform.matWorld_.m[3][3] = 1.0f;
		worldTransform.matWorld_ *=scale(worldTransform.scale_.x, worldTransform.scale_.y, worldTransform.scale_.z);

		// Z軸回転
		worldTransform.matWorld_ *= rotationZ(worldTransform.rotation_.z);

		// X軸回転
		worldTransform.matWorld_ *= rotationX(worldTransform.rotation_.x);

		// Y軸回転
		worldTransform.matWorld_ *= rotationY(worldTransform.rotation_.y);

		//平行移動
		worldTransform.matWorld_ *= translation(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);

		//行列の転送
		worldTransform.TransferMatrix();
	}
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 800);

	//軸方向表示を有効にする

	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するとビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());



}

void GameScene::Update() { debugCamera_->Update(); }

void GameScene::Draw() {

	
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, debugCamera_->GetViewProjection(), textureHandle_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}