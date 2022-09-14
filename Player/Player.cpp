#include "Player.h"
#include "Enemy.h"

using namespace MathUtility;

void Player::Initialize(Model* jikiNormal, Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	modelNormal_ = jikiNormal;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//3Dレティクル用ワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0, 0, -100.0f };

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("tage.png");
	//スプライト生成
	sprite2DReticle_.reset(
		Sprite::Create(textureReticle, Vector2(500, 350), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	bulletModel = Model::CreateFromOBJ("tamaX", true);
	


}

void Player::Update(ViewProjection viewProjection_) {

	//デスフラグの立った弾の削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	playerInvincible--;

	if (playerHp <= 0) {
		isDead_ = true;
	}

	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.8f;
	const float kCharacterSpeed2 = 0.3f;


	// MSと変形機のチェンジ
	oldPlayerChangeMode = isPlayerChange;	//前フレーム処理

	
	if (keyCoolTimeModeChange <= 0) {
		if (input_->TriggerKey(DIK_SPACE)) {
			if (isPlayerChange == 0) {
				isPlayerChange = 1;
				controlAngleX = 0.0f;
				controlAngleY = 0.0f;
				isModeChangeBoost = true;
				keyCoolTimeModeChange = coolTime;
			}
			else {
				isPlayerChange = 0;
				controlAngleX = 0.0f;
				controlAngleY = 0.0f;
				isFly = 1;
				isModeChangeBoost = true;
				gravityVel = 0.0f;
				keyCoolTimeModeChange = coolTime;
			}
		}
	}
	else {
		keyCoolTimeModeChange--;
	}


	Vector3 v1;
	v1 = worldTransform_.translation_ - viewProjection_.eye;
	v1 = MathUtility::Vector3Normalize(v1);
	float atanAngle = atan2f(v1.x, v1.z);

	if (isPlayerChange == 0) {
		angleVelocity = 0.0f;
		if (isPlayerChange != oldPlayerChangeMode) {
			//押した方向で移動ベクトルを変更

			if (input_->PushKey(DIK_W)) {
				speedUpParam.z = kSpeedParamVel;
				isInitAngleMode = 1;
			}
			else if (input_->PushKey(DIK_S)) {
				speedUpParam.z = -kSpeedParamVel;
				isInitAngleMode = 2;
			}
			else if (input_->PushKey(DIK_A)) {
				speedUpParam.x = -kSpeedParamVel;
				speedUpParam.z = 0.3f;
				isInitAngleMode = 3;
			}
			else if (input_->PushKey(DIK_D)) {
				speedUpParam.x = kSpeedParamVel;
				speedUpParam.z = 0.3f;
				isInitAngleMode = 4;

			}
			primaryAngle = atanAngle + angleVelocity;
		}

		//スピード調整
		float kAdjustSpeed = 0.15f;
		if (speedUpParam.x > kSpeedParamVel) {
			speedUpParam.x -= kAdjustSpeed;
		}
		else if (speedUpParam.x < -kSpeedParamVel) {
			speedUpParam.x += kAdjustSpeed;
		}
		if (speedUpParam.y > kSpeedParamVel) {
			speedUpParam.y -= kAdjustSpeed;
		}
		else if (speedUpParam.y < -kSpeedParamVel) {
			speedUpParam.y += kAdjustSpeed;
		}
		if (speedUpParam.z > kSpeedParamVel) {
			speedUpParam.z -= kAdjustSpeed;
		}
		else if (speedUpParam.z < -kSpeedParamVel) {
			speedUpParam.z += kAdjustSpeed;
		}


		isPushTrans = false;
		float kControlSpeed = 0.03f;

		if (keyCoolTimeNormal <= 0) {
			if (input_->TriggerKey(DIK_W)) {
				isPushTrans = true;
				isFly = true;
				primaryAngle = atanAngle + angleVelocity;
				speedUpParam = { 0,0,4.0f };
				gravityVel = 0.0f;
				keyCoolTimeNormal = coolTime;
			}
			else if (input_->TriggerKey(DIK_S)) {
				isPushTrans = true;
				isFly = true;
				primaryAngle = atanAngle + angleVelocity;
				speedUpParam = { 0,0,-4.0f };
				gravityVel = 0.0f;
				keyCoolTimeNormal = coolTime;
			}

			if (input_->TriggerKey(DIK_A)) {
				isPushTrans = true;
				isFly = true;
				primaryAngle = atanAngle + angleVelocity;
				speedUpParam = { -4.0f,0,0 };
				gravityVel = 0.0f;
				keyCoolTimeNormal = coolTime;
			}
			else if (input_->TriggerKey(DIK_D)) {
				isPushTrans = true;
				isFly = true;
				primaryAngle = atanAngle + angleVelocity;
				speedUpParam = { 4.0f,0,0 };
				gravityVel = 0.0f;
				keyCoolTimeNormal = coolTime;
			}
		}
		else {
			keyCoolTimeNormal--;
		}




		Vector3 v2 = { 0,primaryAngle,0 };

		/*const float kCharacterRotateSpeed = 0.05f;
		if (input_->PushKey(DIK_Q)) {
			rotation = { 0, kCharacterRotateSpeed, 0 };
		}
		else if (input_->PushKey(DIK_E)) {
			rotation = { 0, -kCharacterRotateSpeed, 0 };
		}*/

		worldTransform_.rotation_ = v2;

		normalVelocity_ = { speedUpParam.x + boostVelX,speedUpParam.y,speedUpParam.z + boostVelZ };
		Vector3 v3 = normalVelocity_;

		/*float kMinusSpeed = 0.1f;
		if (speedUpParam.x > 0.0f) {
			speedUpParam.x -= kMinusSpeed;
		}
		if (speedUpParam.y > 0.0f) {
			speedUpParam.y -= kMinusSpeed;
		}
		if (speedUpParam.z > 0.0f) {
			speedUpParam.z -= kMinusSpeed;
		}*/

		v3 = bVelocity(v3, worldTransform_);

		worldTransform_.translation_ += v3;

		if (isModeChangeBoost == true) {
			boostCount++;
			if (boostCount <= 7) {
				float kRotSpeed = 0.1f * PI;

				if (isInitAngleMode == 1) {
					speedUpParam.z = 5.0f;

					turnAngle += kRotSpeed;
					worldTransform_.rotation_.x += turnAngle;
				}
				else if (isInitAngleMode == 2) {
					speedUpParam.z = -5.0f;
					turnAngle -= kRotSpeed;
					worldTransform_.rotation_.x += turnAngle;
				}
				else if (isInitAngleMode == 3) {
					speedUpParam.x = -5.0f;
					speedUpParam.y = -0.6f;
					speedUpParam.z = 3.0f;

					turnAngle -= kRotSpeed;
					worldTransform_.rotation_.z += turnAngle;
				}
				else if (isInitAngleMode == 4) {
					speedUpParam.x = 5.0f;
					speedUpParam.y = -0.6f;
					speedUpParam.z = 3.0f;

					turnAngle += kRotSpeed;
					worldTransform_.rotation_.z += turnAngle;
				}
			}
			else {
				isModeChangeBoost = 0;
				turnAngle = 0.0f;
				worldTransform_.rotation_.x = 0;
				worldTransform_.rotation_.z = 0;
				boostCount = 0;
				boostVelZ = 0.0f;
				boostVelX = 0.0f;
			}
		}



		/*if (isPushTrans == true) {

		}
		else {
		}*/
#pragma region 重力
		if (isFly == 1) {
			if (gravityVel >= -8.0f) {
				float kGlavityVel = 0.03f;
				gravityVel -= kGlavityVel;
			}
			if (worldTransform_.matWorld_.m[3][1] > 0.0f) {
				worldTransform_.translation_.y += gravityVel;
			}
			else if (worldTransform_.matWorld_.m[3][1] <= 0.0f) {
				worldTransform_.matWorld_.m[3][1] = 0.0f;
				isFly = 0;
				gravityVel = 0.0f;
			}
		}
		else {	//tyakuti
			float kAdjustVel = 0.03f;
			if (speedUpParam.x > 0) {
				speedUpParam.x -= kAdjustVel;
				
			}
			else if (speedUpParam.x < 0) {
				speedUpParam.x += kAdjustVel;
			}

			if (speedUpParam.y > 0) {
				speedUpParam.y -= kAdjustVel;
			}
			else if (speedUpParam.y < 0) {
				speedUpParam.y += kAdjustVel;
			}

			if (speedUpParam.z > 0) {
				speedUpParam.z -= kAdjustVel;
			}
			else if (speedUpParam.z < 0) {
				speedUpParam.z += kAdjustVel;
			}
		}
#pragma endregion
	}
	else if (isPlayerChange == 1) {
		if (isPlayerChange != oldPlayerChangeMode) {

			//押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_W)) {
				angleVelocity = 0.0f;
				isInitAngleMode = 1;
			}
			else if (input_->PushKey(DIK_S)) {
				angleVelocity = 1.0f * PI;
				isInitAngleMode = 2;
			}
			if (input_->PushKey(DIK_A)) {


				angleVelocity = -0.5f * PI;
				isInitAngleMode = 3;
			}
			else if (input_->PushKey(DIK_D)) {


				angleVelocity = 0.5f * PI;
				isInitAngleMode = 4;


			}


		}
		primaryAngle = atanAngle + angleVelocity;
		isPushTrans = false;
		float kControlSpeed = 0.03f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			isPushTrans = true;
			controlAngleX -= kControlSpeed;
		}
		else if (input_->PushKey(DIK_S)) {
			isPushTrans = true;
			controlAngleX += kControlSpeed;
		}
		/*else {
			if (controlAngleX > 0.0f) {
				controlAngleX -= kControlSpeed;
			}
			if (controlAngleX < 0.0f) {
				controlAngleX += kControlSpeed;
			}
		}*/
		if (input_->PushKey(DIK_A)) {
			isPushTrans = true;
			controlAngleY -= kControlSpeed;
		}
		else if (input_->PushKey(DIK_D)) {
			isPushTrans = true;
			controlAngleY += kControlSpeed;

		}
		/*else {
			if (controlAngleY > 0.0f) {
				controlAngleY -= kControlSpeed;
			}
			if (controlAngleY < 0.0f) {
				controlAngleY += kControlSpeed;
			}
		}*/

		Vector3 v2 = { controlAngleX,primaryAngle + controlAngleY,0 };

		/*const float kCharacterRotateSpeed = 0.05f;
		if (input_->PushKey(DIK_Q)) {
			rotation = { 0, kCharacterRotateSpeed, 0 };
		}
		else if (input_->PushKey(DIK_E)) {
			rotation = { 0, -kCharacterRotateSpeed, 0 };
		}*/

		

		worldTransform_.rotation_ = v2;

		

		if (isModeChangeBoost == true) {
			boostCount++;
			if (boostCount <= 12) {
				float kRotSpeed = 0.1f * PI;

				if (isInitAngleMode == 1) {
					
					turnAngle += kRotSpeed;
					worldTransform_.rotation_.x = -0.5f * PI;
					//worldTransform_.rotation_.z += turnAngle;

				}
				//else if (isInitAngleMode == 2) {
				//	speedUpParam.z = 10.0f;
				//	turnAngle -= kRotSpeed;
				//	worldTransform_.rotation_.x = -0.5f * PI;
				//	//worldTransform_.rotation_.z += turnAngle;
				//}
				//else if (isInitAngleMode == 3) {
				//	speedUpParam.z = 10.0f;

				//	turnAngle -= kRotSpeed;
				//	worldTransform_.rotation_.x = -0.5f * PI;
				//	//worldTransform_.rotation_.z += turnAngle;
				//}
				//else if (isInitAngleMode == 4) {
				//	speedUpParam.z = 10.0f;

				//	turnAngle += kRotSpeed;
				//	worldTransform_.rotation_.x = -0.5f * PI;
				//	//worldTransform_.rotation_.z += turnAngle;
				//}
			}
			else {
				isModeChangeBoost = 0;
				turnAngle = 0.0f;
				worldTransform_.rotation_.x = 0;
				worldTransform_.rotation_.z = 0;
				boostCount = 0;
				boostVelZ = 0.0f;
				boostVelX = 0.0f;
			}
		}


		Vector3 v3 = { 0,0.0f,1.5f };
		

		if (isModeChangeBoost == true) {
			worldTransform_.rotation_.z = turnAngle;
			v3.z = 5.0f;
		}
		v3 = bVelocity(v3, worldTransform_);
		worldTransform_.translation_ += v3;
		
		
	}

	if (worldTransform_.matWorld_.m[3][1] < 0.0f) {	//地面に潜らない
		worldTransform_.translation_.y = 0.0f;;
	}
	

	//行列更新
	//AffinTrans::affin(worldTransform_);
	worldTransform_.matWorld_ = AffinTrans::Initialize();
	worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= AffinTrans::Rotation(worldTransform_.rotation_, 3);
	worldTransform_.matWorld_ *= AffinTrans::Rotation(worldTransform_.rotation_, 1);
	worldTransform_.matWorld_ *= AffinTrans::Rotation(worldTransform_.rotation_, 2);
	
	worldTransform_.matWorld_ *= AffinTrans::Move(worldTransform_.translation_);

	//worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();


	/*const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;*/

	////範囲を超えない処理
	//worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	//worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	//worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	//worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	const float kChestRotSpeed = 0.05f;


	//弾発射処理
	Attack();

	//弾更新
	//複数
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//単発
	/*if (bullet_) {
		bullet_->Update();
	}*/

	//---------自機のワールド座標から3Dレティクルのワールド座標を計算-----------//

	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 60.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0, 0, 2.0f };
	//自機のワールド座標の回転を反映
	offset = AffinTrans::MatVector(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = Vector3Normalize(offset) * kDistancePlayerTo3DReticle;
	//3Dレティクル座標設定
	worldTransform3DReticle_.translation_ =
		offset + Vector3(
			worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2]
		);
	//行列更新
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

	/////////////////////////////////////////////////////////////////


	//----------3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算-------//

	Vector3 positionReticle = AffinTrans::GetWorldtransform(worldTransform3DReticle_.matWorld_);

	Vector2 windowWH =
		Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	//ビューポート行列
	Matrix4 Viewport = {
	   windowWH.x / 2,				  0,  0,  0,
					0,	-windowWH.y / 2,  0,  0,
					0,				  0,  1,  0,
	   windowWH.x / 2,	 windowWH.y / 2,  0,  1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = viewProjection_.matView * viewProjection_.matProjection * Viewport;

	//ワールド→スクリーン座標変換(ここから3Dから2Dになる)
	positionReticle = AffinTrans::wDivision(positionReticle, matViewProjectionViewport);

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	////////////////////////////////////////////////////////////////////////////////

	//----------スクリーン座標から2Dレティクルのワールド座標を計算-------//

	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	//ビュープロジェクションビューポート合成
	Matrix4 matVPV = viewProjection_.matView * viewProjection_.matProjection * Viewport;

	//合成行列の逆行列を計算する
	Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	//スクリーン座標
	Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = AffinTrans::wDivision(posNear, matInverseVPV);
	posFar = AffinTrans::wDivision(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Vector3Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_ = AffinTrans::AddVector(posNear, mouseDirection * kDistanceTestObject);

	//行列更新
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

	/////////////////////////////////////////////////////////////


	/*debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "translation : %f,%f,%f", worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	DebugText::GetInstance()->SetPos(20, 200);
	DebugText::GetInstance()->Printf("Mouse ScreenPos:(%d,%d)", mousePosition.x, mousePosition.y);
	DebugText::GetInstance()->SetPos(20, 220);
	DebugText::GetInstance()->Printf("Near:(%f,%f,%f)", posNear.x, posNear.y, posNear.z);
	DebugText::GetInstance()->SetPos(20, 240);
	DebugText::GetInstance()->Printf("Far:(%f,%f,%f)", posFar.x, posFar.y, posFar.z);
	DebugText::GetInstance()->SetPos(20, 260);
	DebugText::GetInstance()->Printf(
	  "MouseObject:(%f,%f,%f)", worldTransform3DReticle_.translation_.x,
	  worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);*/
	  //デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf("Change:%d", isPlayerChange);

	debugText_->SetPos(50, 170);
	debugText_->Printf("PlayerHp:%d", playerHp);

	debugText_->SetPos(50, 190);
	debugText_->Printf("PlayerHp:%d", playerInvincible);
}

void Player::Draw(ViewProjection viewProjection_) {
	if (isPlayerChange == 1) {
		model_->Draw(worldTransform_, viewProjection_);
	}
	else if (isPlayerChange == 0) {
		modelNormal_->Draw(worldTransform_, viewProjection_);
	}

	//弾更新
	//複数
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}


	////3Dレティクルを描画
	//model_->Draw(worldTransform3DReticle_, viewProjection_,textureHandle_);




	//単発
	/*if (bullet_) {
		bullet_->Draw(viewProjection_);
	}*/
}

void Player::Attack() {
	if (input_->IsTriggerMouse(0)) {
		if (isPlayerChange == 0) {
			//弾の速度
			const float kBulletSpeed = 20.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			//速度ベクトルを自機の向きに合わせて回転させる
			velocity = bVelocity(velocity, worldTransform_);

			////自機から標準オブジェクトへのベクトル
			//velocity = AffinTrans::GetWorldtransform(worldTransform3DReticle_.matWorld_) - AffinTrans::GetWorldtransform(worldTransform_.matWorld_);
			//velocity = Vector3Normalize(velocity) * kBulletSpeed;

			//	//弾の速度
			//const float kBulletSpeed = 15.0f;

			//Vector3 velocity(0, 0, kBulletSpeed);

			//プレイヤーのワールド座標の取得
			Vector3 playerPosition;
			playerPosition = GetWorldPosition2();
			//敵のワールド座標を取得
			//差分ベクトルを求める
			Vector3 A_BVec = Vector3(
				enemyPos_.x - playerPosition.x, enemyPos_.y - playerPosition.y,
				enemyPos_.z - playerPosition.z);
			//ベクトル正規化
			float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z) * 10;
			//ベクトルの長さを速さに合わせる
			A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);


		//単発
		/*PlayerBullet* newBullet = new PlayerBullet();*/
		newBullet->Initialize(bulletModel, AffinTrans::GetWorldtransform(worldTransform_.matWorld_), velocity);


			A_BVec *= kBulletSpeed;


			//弾を生成し初期化
			//複数
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

			//単発
			/*PlayerBullet* newBullet = new PlayerBullet();*/
			newBullet->Initialize(model_, AffinTrans::GetWorldtransform(worldTransform_.matWorld_), A_BVec);

			//弾の登録
		   //複数
			bullets_.push_back(std::move(newBullet));

			//単発
			/*bullet_.reset(newBullet);*/
		}
	}

}

void Player::DrawUI() {
	sprite2DReticle_->Draw();

}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) {

	Vector3 result = { 0, 0, 0 };


	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];


	return result;
}

Vector3 Player::GetWorldPosition2() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {
	if (playerInvincible < 0) {
		playerHp--;
		playerInvincible = 30;
	}
	
}

void Player::setparent(WorldTransform* worldTransform) {
	worldTransform_.parent_ = worldTransform;
}

void Player::ResetPlayer()
{
	//デスフラグの立った弾の削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->OnCollision();
	}
	playerHp = 3;
	isDead_ = false;
	playerInvincible = 0;
}


