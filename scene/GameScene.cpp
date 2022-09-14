#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelPlayer1_;
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
	
	

	//レティクルのテクスチャ
	TextureManager::Load("tage.png");

	model_ = Model::Create();
	modelPlayer1_ = Model::CreateFromOBJ("JikiHenkei2", true);
	modelPlayer2_ = Model::CreateFromOBJ("Jiki", true);
	modelField1_ = Model::CreateFromOBJ("field1", true);
	enemyBox_ = Model::CreateFromOBJ("EnemyBox", true);

	//レティクルのテクスチャ
	uint32_t texture = TextureManager::Load("RedReticle3.png");
	spriterock.reset(
	  Sprite::Create(texture, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	//タイトルのテクスチャ
	uint32_t title = TextureManager::Load("title.png");
	spriteTitle.reset(
	  Sprite::Create(title, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	//クリアのテクスチャ
	uint32_t clear = TextureManager::Load("clear.png");
	spriteClear.reset(
	  Sprite::Create(clear, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	//タイトルのテクスチャ
	uint32_t over = TextureManager::Load("over.png");
	spriteOver.reset(
	  Sprite::Create(over, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	//UI
	uint32_t ui = TextureManager::Load("UI.png");
	spriteUI.reset(Sprite::Create(ui, Vector2(1130, 570), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	
	//GO
	uint32_t goText1 = TextureManager::Load("GO1.png");
	uint32_t goText2 = TextureManager::Load("GO2.png");
	spriteGO1.reset(
		Sprite::Create(goText1, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteGO2.reset(
		Sprite::Create(goText2, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(modelPlayer2_, modelPlayer1_, textureHandle_);
	player_->setparent(railCamera_->GetWorldPosition());

	

	//スカイドームの生成
	skydome_ = new Skydome();
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	
	//スカイドームの初期化
	skydome_->Initialize(modelSkydome_);

	//フィールド生成
	fieldObj_ = new FieldObj();
	fieldObj_->Initialize(modelField1_);

	//地面の生成
	ground_ = new Ground();
	// 3Dモデルの生成
	modelGround_ = Model::CreateFromOBJ("Ground", true);
	//地面の初期化
	ground_->Initialize(modelGround_);

	//レールカメラ
	railCamera_ = new RailCamera();
	//レールカメラの初期化
	railCamera_->Initialize(Vector3(0, 10, 30), Vector3(0, 0, 0));

	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = GetBullets();
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の衝突時コールバックを呼び出す
		bullet->OnCollision();
	}

	//ファイルの読み込み
	LoadEnemyPopData();

	audio_ = Audio::GetInstance();
	bgmHandle = audio_->LoadWave("title.wav");
	bgmHandle2 = audio_->LoadWave("battle.wav");
	bgmHandle3 = audio_->LoadWave("clear.wav");
	bgmHandle4 = audio_->LoadWave("over.wav");


	goTexPosX = 2.0f;

}

void GameScene::Update() {
	
	spriteGO1->SetPosition(Vector2(-goTexPosX + 600, 360.0f));
	spriteGO2->SetPosition(Vector2(goTexPosX + 600, 360.0f));
	//スプライトの今の座標を取得
	// XMFLOAT2 position = sprite->GetPosition();
	//座標を｛２，０｝移動
	// position.x += 2.0f;
	// position.y += 1.0f;

	//デスフラグの立った弾の削除
	enemybullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
	//デスフラグの立った弾の削除
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& playerbullet) { return playerbullet->IsDead(); });

	//デスフラグの立った敵の削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });


	//デスフラグの立ったエフェクトの削除
	effects_.remove_if([](std::unique_ptr<Effect>& effect) { return effect->IsDead(); });

	switch (sceneNo_) {
	case SceneNo::Title: //タイトル
		if (input_->IsTriggerMouse(1) && sceneNo_ == SceneNo::Title) {
			sceneNo_ = SceneNo::Game;
			EnemyReset();
			playerTimer = 1000;
			enemyDefeat = 0;
		}
		EnemyReset();
		playerTimer = 1000;
		enemyDefeat = 0;
		

		break;
	case SceneNo::Game: //射撃

						//自機のHPタイマー
		playerTimer--;

		//自キャラの更新
		player_->setparent(railCamera_->GetWorldPosition());
		player_->Update(railCamera_->GetViewProjection());

		//敵発生
		UpdataEnemyPopCommands();

		if (input_->TriggerKey(DIK_E)) {
			targetChange++;
		}
		noEnemy = 0;

		cameraFlag_ = 0;

		//敵の更新
		for (std::unique_ptr<Enemy>& enemy_ : enemys_) {

			enemy_->SetGameScene(this);
			enemy_->Update();

			//デバッグ用表示

			if (targetChange > targetMax || goThrough == 1) {
				targetChange = enemy_->GetId();
				goThrough = 0;
			}

			if (enemy_->GetId() == targetChange) {
				EnemyTarget(enemy_->GetWorldPosition(), player_->GetWorldPosition2(), 2);
				noEnemy++;
			}
			//else if (targetChange != enemy_->GetId() && goThrough >= 1) {
			//	targetChange++;
			//}


		}
		if (noEnemy == 0) {
			targetChange++;
		}
		debugText_->SetPos(50, 110);
		debugText_->Printf("targetNum :%d", targetMax);

		//	EnemyTarget(enemy_->GetWorldPosition(), player_->GetWorldPosition2(), 2);
		//	


		//	cameraFlag_ = 1;
		//}

		//if (cameraFlag_ == 0) {
		//	Vector3 p = player_->GetWorldPosition2();
		//	Vector3 e = lastEnemyPos;
		//	Vector3 playerTarget = {e.x - p.x, e.y - p.y, e.z - p.z};

		//	Vector3 PosNorm = MathUtility::Vector3Normalize(playerTarget);
		//	float len = 50.0f;
		//	Vector3 cameraPos = {
		//	  p.x - PosNorm.x * len, (p.y - PosNorm.y * len) + 10.0f, p.z - PosNorm.z * len};

		//	//カメラの位置制御
		//	railCamera_->GetViewProjection().eye = cameraPos;
		//}

		//弾更新
		//複数
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
			bullet->Update();
		}
		for (std::unique_ptr<Effect>& effect : effects_) {
			effect->Update();
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
		/*debugText_->SetPos(50, 110);
		debugText_->Printf("Time limit :%d", targetChange);*/

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

		for (std::unique_ptr<Effect>& effect : effects_) {
			effect->effectDead();
		}
		player_->ResetPlayer();
		railCamera_->ResetRailCamera();
		/*gameClear_->Update();
		push_->Update();*/

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

		for (std::unique_ptr<Effect>& effect : effects_) {
			effect->effectDead();
		}

		player_->ResetPlayer();
		railCamera_->ResetRailCamera();
		/*gameOver_->Update();
		push_->Update();*/

		break;
	}

	if (!enemys_.empty())
	{
		debugText_->SetPos(10, 30);
		debugText_->Printf("%f:%f:%f", enemys_.front().get()->GetWorldPosition().x, enemys_.front().get()->GetWorldPosition().y, enemys_.front().get()->GetWorldPosition().z);
	}
	debugText_->SetPos(10, 80);
	debugText_->Printf("%d", targetMax);
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
		audio_->StopWave(soundHandle3);
		audio_->StopWave(soundHandle4);
		if (audio_->IsPlaying(soundHandle) == 0 || soundHandle == -1) {
			soundHandle = audio_->PlayWave(bgmHandle, true, 0.5f);
		}
		
		break;
	case SceneNo::Game: //射撃
		audio_->StopWave(soundHandle);

		if (audio_->IsPlaying(soundHandle2) == 0 || soundHandle2 == -1) {
			soundHandle2 = audio_->PlayWave(bgmHandle2, true, 0.5f);
		}


		//ground_->Draw(railCamera_->GetViewProjection());
		fieldObj_->Draw(railCamera_->GetViewProjection());

		player_->Draw(railCamera_->GetViewProjection());

		for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
			enemy_->Draw(railCamera_->GetViewProjection());
		}

		//弾更新
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets_) {
			bullet->Draw(railCamera_->GetViewProjection());
		}
		for (std::unique_ptr<Effect>& effect : effects_) {
			effect->Draw(railCamera_->GetViewProjection());
		}
		break;
	case SceneNo::Clear: //クリア
		audio_->StopWave(soundHandle2);
		if (audio_->IsPlaying(soundHandle3) == 0 || soundHandle3 == -1) {
			soundHandle3 = audio_->PlayWave(bgmHandle3, true, 0.5f);
		}
		break;
	case SceneNo::Over: //オーバー
		audio_->StopWave(soundHandle2);
		if (audio_->IsPlaying(soundHandle4) == 0 || soundHandle4 == -1) {
			soundHandle4 = audio_->PlayWave(bgmHandle4, true, 0.5f);
		}
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
		spriteTitle->Draw();
		break;
	case SceneNo::Game: //射撃
		player_->DrawUI();
		if (enemys_.size() >= 1) {
			spriterock->Draw();
		}
		spriteUI->Draw();
		goTexPosX *= 1.2;
		if (goTexPosX < 800) {
			spriteGO1->Draw();	//GO
			spriteGO2->Draw();
		}

		break;
	case SceneNo::Clear: //クリア
		spriteClear->Draw();
		break;
	case SceneNo::Over: //オーバー
		spriteOver->Draw();
		break;
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

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
				//敵キャラの衝突時コールバックを呼び出す
				enemy_->OnCollision();
				if (enemy_->GetId() == targetChange) {
					/*targetChange = enemy_->GetId() + 1;*/
					goThrough++;
				}
				enemyDefeat++;
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}

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

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet) {
	//リストに登録する
	enemybullets_.push_back(std::move(enemyBullet));
}

void GameScene::AddEffect(std::unique_ptr<Effect>& efect)
{
	//リストに登録する
	effects_.push_back(std::move(efect));
}

void GameScene::LoadEnemyPopData() {

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop2.csv");

	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

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

			//ID
			std::getline(line_stream, word, ',');
			int ID = static_cast<int>(std::atof(word.c_str()));

			targetMax = ID;

			GenerEnemy(Vector3(x, y, z), ID);
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

void GameScene::GenerEnemy(Vector3 EnemyPos, int ID) {
	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//敵キャラの初期化
	newEnemy->Initialize(enemyBox_, textureHandle2_, EnemyPos);

	//敵キャラにアドレスを渡す
	newEnemy->SetPlayer(player_);

	newEnemy->SetID(ID);

	//リストに登録する
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::EnemyReset() {
	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();
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

	float len = 50.0f;
	Vector3 cameraPos = {
	  playerPos.x - PosNorm.x * len, (playerPos.y - PosNorm.y * len) + 11.0f,
	  playerPos.z - PosNorm.z * len};
	lastEnemyPos = cameraPos;


	//カメラの位置制御
	railCamera_->GetViewProjection().eye = cameraPos;
}



Vector3 GameScene::vector3(float x, float y, float z) { return Vector3(x, y, z); }

Vector4 GameScene::vector4(int x, int y, int z, int w) { return Vector4(x, y, z, w); }