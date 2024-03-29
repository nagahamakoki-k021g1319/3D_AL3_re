﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete modelSkydome_;
	delete modelGround_;
	delete debugCamera_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	textureHandle_ = TextureManager::Load("mario.jpg");
	textureHandle2_ = TextureManager::Load("enemy.jpg");
	textureHandle3_ = TextureManager::Load("title.png");
	textureHandle0_ = TextureManager::Load("kuriku.png");
	textureHandle4_ = TextureManager::Load("claer.png");
	textureHandle5_ = TextureManager::Load("over.png");
	//レティクルのテクスチャ
	TextureManager::Load("tage.png");
	model_ = Model::Create();

	//レティクルのテクスチャ
	uint32_t texture = TextureManager::Load("RedReticle2.png");
	spriterock.reset(
	  Sprite::Create(texture, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	player_->setparent(railCamera_->GetWorldPosition());

	//タイトルの生成
	title_ = new Title();
	//タイトルの初期化
	title_->Initialize(model_, textureHandle3_);

	//クリックの生成
	push_ = new push();
	//クリックの初期化
	push_->Initialize(model_, textureHandle0_);

	//ゲームクリアの生成
	gameClear_ = new GameClear();
	//ゲームクリアの初期化
	gameClear_->Initialize(model_, textureHandle4_);

	//ゲームオーバーの生成
	gameOver_ = new GameOver();
	//ゲームオーバーの初期化
	gameOver_->Initialize(model_, textureHandle5_);

	//スカイドームの生成
	skydome_ = new Skydome();
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//スカイドームの初期化
	skydome_->Initialize(modelSkydome_);

	//地面の生成
	ground_ = new Ground();
	// 3Dモデルの生成
	modelGround_ = Model::CreateFromOBJ("Ground", true);
	//地面の初期化
	ground_->Initialize(modelGround_);

	//レールカメラ
	railCamera_ = new RailCamera();
	//レールカメラの初期化
	railCamera_->Initialize(Vector3(0, 0, 20), Vector3(0, 0, 0));

	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = GetBullets();
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の衝突時コールバックを呼び出す
		bullet->OnCollision();
	}

	//ファイルの読み込み
	LoadEnemyPopData();
	LoadEnemyPopData2();

	audio_ = Audio::GetInstance();
	bgmHandle = audio_->LoadWave("fanfare.wav");
}

void GameScene::Update() {
	//スプライトの今の座標を取得
	// XMFLOAT2 position = sprite->GetPosition();
	//座標を｛２，０｝移動
	// position.x += 2.0f;
	// position.y += 1.0f;

	//デスフラグの立った弾の削除
	enemybullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->InDead(); });

	//デスフラグの立った敵の削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });

	switch (sceneNo_) {
	case SceneNo::Title: //タイトル
		if (input_->IsTriggerMouse(1) && sceneNo_ == SceneNo::Title) {
			sceneNo_ = SceneNo::Operate;
			EnemyReset();
			playerTimer = 1000;
			enemyDefeat = 0;
		}
		title_->Update();

		push_->Update();
		EnemyReset();
		playerTimer = 1000;
		enemyDefeat = 0;

		break;
	case SceneNo::Operate: //操作説明(チュートリアル)
		if (input_->IsTriggerMouse(1) && sceneNo_ == SceneNo::Operate) {
			EnemyReset2();
			/*for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
				enemy_->OnCollision();
			}*/
			playerTimer = 1000;
			enemyDefeat = 0;
			sceneNo_ = SceneNo::Game;
			
		}
	
		//自キャラの更新
		player_->setparent(railCamera_->GetWorldPosition());
		player_->Update(railCamera_->GetViewProjection());

		//敵発生
		UpdataEnemyPopCommands2();
		cameraFlag_ = 0;
		//敵の更新
		for (std::unique_ptr<Enemy>& enemy_ : enemys_) {

			enemy_->SetGameScene(this);
			enemy_->Update();
			EnemyTarget(enemy_->GetWorldPosition(), player_->GetWorldPosition2(), 2);

			cameraFlag_ = 1;
		}

		if (cameraFlag_ == 0) {
			Vector3 p = player_->GetWorldPosition2();
			Vector3 e = lastEnemyPos;
			Vector3 playerTarget = {e.x - p.x, e.y - p.y, e.z - p.z};

			Vector3 PosNorm = MathUtility::Vector3Normalize(playerTarget);
			float len = 30.0f;
			Vector3 cameraPos = {
			  p.x - PosNorm.x * len, (p.y - PosNorm.y * len) + 6.0f, p.z - PosNorm.z * len};

			//カメラの位置制御
			railCamera_->GetViewProjection().eye = cameraPos;
		}
		//弾更新
		//複数
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
			bullet->Update();
		}

		CheckAllCollisions();

		//ゲームクリアに突入
		if (enemyDefeat >= 4) {
			sceneNo_ = SceneNo::Clear;
		}

		//レールカメラの更新
		railCamera_->Update();

		break;
	case SceneNo::Game: //射撃

		playerTimer--; //自機のHPタイマー

		//自キャラの更新
		player_->setparent(railCamera_->GetWorldPosition());
		player_->Update(railCamera_->GetViewProjection());

		//敵発生
		UpdataEnemyPopCommands();
		cameraFlag_ = 0;
		//敵の更新
		for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
			enemy_->SetGameScene(this);
			enemy_->Update();
			EnemyTarget(enemy_->GetWorldPosition(), player_->GetWorldPosition2(), 2);
		
			cameraFlag_ = 1;
		}

		if (cameraFlag_ == 0) {
			Vector3 p = player_->GetWorldPosition2();
			Vector3 e = lastEnemyPos;
			Vector3 playerTarget = {e.x - p.x, e.y - p.y, e.z - p.z};

			Vector3 PosNorm = MathUtility::Vector3Normalize(playerTarget);
			float len = 30.0f;
			Vector3 cameraPos = {
			  p.x - PosNorm.x * len, (p.y - PosNorm.y * len) + 6.0f, p.z - PosNorm.z * len};

			//カメラの位置制御
			railCamera_->GetViewProjection().eye = cameraPos;
		}
		//弾更新
		//複数
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
			bullet->Update();
		}

		CheckAllCollisions();

		//ゲームクリアに突入
		if (enemyDefeat >= 4) {
			sceneNo_ = SceneNo::Clear;
		}

		////ゲームオーバーに突入
		// if (playerTimer <= 0) {
		//	sceneNo_ = SceneNo::Over;
		// }

		/*railCamera_->GetViewProjection().target = { player_->GetWorldPosition2() };*/

		//レールカメラの更新
		railCamera_->Update();

		//デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("Time limit :%d", playerTimer);

		break;
	case SceneNo::Clear: //クリア
		for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
			enemy_->OnCollision();
		}
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
			bullet->OnCollision();
		}
		if (input_->IsTriggerMouse(1) && sceneNo_ == SceneNo::Clear) {
			sceneNo_ = SceneNo::Title;
		}

		gameClear_->Update();
		push_->Update();
		break;
	case SceneNo::Over: //オーバー
		for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
			enemy_->OnCollision();
		}
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
			bullet->OnCollision();
		}
		if (input_->IsTriggerMouse(1) && sceneNo_ == SceneNo::Over) {
			sceneNo_ = SceneNo::Title;
		}

		gameOver_->Update();
		push_->Update();
		break;
	}

	//デバッグ用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%b,%d,%f)", audio_->IsPlaying(bgmHandle), soundHandle == -1, viewProjection_.up.z);
#pragma endregion
}

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

	skydome_->Draw(railCamera_->GetViewProjection());

	switch (sceneNo_) {
	case SceneNo::Title: //タイトル
		title_->Draw(viewProjection_);
		push_->Draw(viewProjection_);
		if (audio_->IsPlaying(soundHandle) == 0 || soundHandle == -1) {
			soundHandle = audio_->PlayWave(bgmHandle, true, 0.5f);
		}
		break;
	case SceneNo::Operate: //操作説明(チュートリアル)
		audio_->StopWave(soundHandle);
		ground_->Draw(railCamera_->GetViewProjection());
		player_->Draw(railCamera_->GetViewProjection());
		for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
			enemy_->Draw(railCamera_->GetViewProjection());
		}

		//弾更新
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
			bullet->Draw(railCamera_->GetViewProjection());
		}
		break;
	case SceneNo::Game: //射撃
		ground_->Draw(railCamera_->GetViewProjection());
		player_->Draw(railCamera_->GetViewProjection());
		for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
			enemy_->Draw(railCamera_->GetViewProjection());
		}

		//弾更新
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
			bullet->Draw(railCamera_->GetViewProjection());
		}
		break;
	case SceneNo::Clear: //クリア
		gameClear_->Draw(viewProjection_);
		push_->Draw(viewProjection_);
		break;
	case SceneNo::Over: //オーバー
		gameOver_->Draw(viewProjection_);
		push_->Draw(viewProjection_);
		break;
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

	switch (sceneNo_) {
	case SceneNo::Title: //タイトル
		break;
	case SceneNo::Operate: //操作説明(チュートリアル)
		player_->DrawUI();
		if (enemys_.size() >= 1) {
			spriterock->Draw();
		}
		break;
	case SceneNo::Game: //射撃
		player_->DrawUI();
		if (enemys_.size() >= 1) {
			spriterock->Draw();
		}
		break;
	case SceneNo::Clear: //クリア
		break;
	case SceneNo::Over: //オーバー
		break;
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

	
#pragma endregion

	//#pragma region 自弾と敵弾の当たり判定
	////自キャラと敵弾すべての当たり判定
	// for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
	//	for (const std::unique_ptr<EnemyBullet>& bullet2 : enemyBullets) {

	//		//自弾の座標
	//		posA = bullet.get()->GetWorldPosition();

	//		//敵弾の座標
	//		posB = bullet2.get()->GetWorldPosition();

	//		float x = posB.x - posA.x;
	//		float y = posB.y - posA.y;
	//		float z = posB.z - posA.z;

	//		float cd = sqrt(x * x + y * y + z * z);

	//		if (cd <= playerBulletRadius + enemyBulletRadius) {
	//			//自キャラの衝突時コールバックを呼び出す
	//			bullet->OnCollision();
	//			//敵弾の衝突時コールバックを呼び出す
	//			bullet2->OnCollision();
	//		}
	//	}
	//}
	//#pragma endregion
}

void GameScene::CheckAllCollisions2() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition2();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		float x = posB.x - posA.x;
		float y = posB.y - posA.y;
		float z = posB.z - posA.z;

		float cd = sqrt(x * x + y * y + z * z);

		if (cd <= playerRadius + enemyBulletRadius) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		posA = enemy_->GetWorldPosition();

		//自弾と敵キャラの当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet.get()->GetWorldPosition();

			float x = posB.x - posA.x;
			float y = posB.y - posA.y;
			float z = posB.z - posA.z;

			float cd = sqrt(x * x + y * y + z * z);

			if (cd <= playerBulletRadius + enemyRadius) {
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet) {
	//リストに登録する
	enemybullets_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData() {

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");

	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::LoadEnemyPopData2() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop2.csv");

	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands2 << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdataEnemyPopCommands() {

	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			GenerEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			standTime_ = waitTime;

			//抜ける
			break;
		}
	}
}

void GameScene::UpdataEnemyPopCommands2() {
	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands2, line)) {
		// 1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			GenerEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			standTime_ = waitTime;

			//抜ける
			break;
		}
	}
}

void GameScene::GenerEnemy(Vector3 EnemyPos) {
	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//敵キャラの初期化
	newEnemy->Initialize(model_, textureHandle2_, EnemyPos);

	//敵キャラにアドレスを渡す
	newEnemy->SetPlayer(player_);

	//リストに登録する
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::EnemyReset() {
	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();
}

void GameScene::EnemyReset2() {
	enemyPopCommands2.str("");
	enemyPopCommands2.clear(std::stringstream::goodbit);
	LoadEnemyPopData2();
}

void GameScene::EnemyTarget(Vector3 targetPos, Vector3 playerPos, float distance) {

	//単位ベクトルの取得
	Vector3 playerTarget = {
	  targetPos.x - playerPos.x, targetPos.y - playerPos.y, targetPos.z - playerPos.z};
	float length =
	  sqrtf(powf(playerTarget.x, 2.0f) + powf(playerTarget.y, 2.0f) + powf(playerTarget.z, 2.0f));
	Vector3 unitvecPlayerTarget = {
	  playerTarget.x / length, playerTarget.y / length, playerTarget.z / length};

	//注視点取得
	railCamera_->GetViewProjection().target = {
	  targetPos.x + unitvecPlayerTarget.x, targetPos.y + unitvecPlayerTarget.y,
	  targetPos.z + unitvecPlayerTarget.z};

	Vector3 PosNorm = MathUtility::Vector3Normalize(playerTarget);
	float len = 30.0f;
	Vector3 cameraPos = {
	  playerPos.x - PosNorm.x * len, (playerPos.y - PosNorm.y * len) + 6.0f,
	  playerPos.z - PosNorm.z * len};
	lastEnemyPos = cameraPos;

	//カメラの位置制御
	railCamera_->GetViewProjection().eye = cameraPos;
}



Vector3 GameScene::vector3(float x, float y, float z) { return Vector3(x, y, z); }

Vector4 GameScene::vector4(int x, int y, int z, int w) { return Vector4(x, y, z, w); }