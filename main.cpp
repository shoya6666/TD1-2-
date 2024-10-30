#include <Novice.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "GC1C_02_キム_ジュンス";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

typedef struct Vector2 {
	float x;
	float y;
} Vector2;


typedef struct Player {
	Vector2 position;
	float width;
	float height;
	float speedX;
	float speedY;
	int radius;
	Vector2 moveSpeed;
	float theta;
	Vector2 acceleration;
	Vector2 velocity;
	int isJump;
	int isSecondJump;
	float gravity;
	int onGround;
	int shootCoolTime;
	int hp;
	int hpGauge;
} Player;

typedef struct Bullet {
	Vector2 pos;
	float width;
	float height;
	float radius;
	float speed;
	int isShoot;
} Bullet;



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player player;
	player.position.x = 200.0f;//プレイヤーのX座標
	player.position.y = 635 - 74;//プレイヤーのY座標
	player.width = 64;
	player.height = 64;
	player.speedX = 0.0f;//プレイヤーのX方向の移動量
	player.speedY = 0.0f; //プレイヤーのY方向の移動量
	player.radius = 32;
	player.moveSpeed.x = 5.0f; //プレイヤーの移動速度
	player.moveSpeed.y = 5.0f; //プレイヤーの移動速度
	player.theta = 0.0f;
	player.acceleration.y = -0.8f;
	player.acceleration.x = 0.0f;
	player.velocity.x = 0.0f;
	player.velocity.y = 0.0f;
	player.isJump = false;
	player.isSecondJump = false;
	player.gravity = 0.4f;
	player.onGround = true;
	player.shootCoolTime = 10;
	player.hp = 300;
	player.hpGauge = 350;

	// 弾の変数
	Bullet bullet[32];
	for (int i = 0; i < 32; i++) {
		bullet[i].pos.x;
		bullet[i].pos.y;
		bullet[i].width = 8.0f;
		bullet[i].height = 16.0f;
		bullet[i].radius = 8.0f;
		bullet[i].speed = 15.0f;
		bullet[i].isShoot = false;
	}

	int groundPosY = 635;

	int genjiGraphHandle = Novice::LoadTexture("./NoviceResources/genji.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		player.speedX = 0;
		player.speedY = 0;


		//キーによって移動方向を設定
		if (keys[DIK_LEFT]) {
			player.speedX = -player.moveSpeed.x;
		}
		if (keys[DIK_RIGHT]) {
			player.speedX = player.moveSpeed.x;
		}
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] && !player.isSecondJump && player.isJump) {
			player.velocity.y = 4.0f;
			player.isSecondJump = true;
		}
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] && !player.isJump && !player.isSecondJump) {
			player.velocity.y = 6.0f;
			player.isJump = true;
			player.isSecondJump = false;
		}

		if (player.isJump || player.isSecondJump) {
			player.velocity.y -= player.gravity;
			player.position.y -= player.moveSpeed.y + player.velocity.y;
			player.onGround = false;
		}
		if (player.position.y >= groundPosY - player.height - 10) {
			player.onGround = true;
			player.isJump = false;
			player.isSecondJump = false;
		}
		player.position.x += player.speedX;

		if (player.shootCoolTime > 0) {
			player.shootCoolTime--;
		}
		else {
			player.shootCoolTime = 10;
		}
		if (keys[DIK_D]) {

			// 発射間隔の調整用クールタイムの計算

			if (player.shootCoolTime <= 0) {
				for (int i = 0; i < 32; i++) {
					if (!bullet[i].isShoot) {
						bullet[i].isShoot = true;
						bullet[i].pos.x = player.position.x+player.width;
						bullet[i].pos.y = player.position.y+player.height/2;
						break;
					}
				}
			}
		}

		// 弾道計算
		for (int i = 0; i < 32; i++) {
			if (bullet[i].isShoot) {
				// 上方向に進ませる
				bullet[i].pos.x += bullet[i].speed;

				// 画面外に出たら発射フラグをFalseに変更する
				if (bullet[i].pos.x >= 1280 - bullet[i].height / 2.0f) {
					bullet[i].isShoot = false;
				}
			}
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		/// 
		/// 
		for (int i = 0; i < 32; i++) {
			if (bullet[i].isShoot) {
				Novice::DrawEllipse(
					static_cast<int>(bullet[i].pos.x),static_cast<int>(bullet[i].pos.y),10,10,0.0f, RED ,kFillModeSolid);
			}
		}

		Novice::DrawBox(30, 350, 10, 302, 0, WHITE, kFillModeWireFrame);
		Novice::DrawBox(31, player.hpGauge, 8, player.hp, 0, RED, kFillModeSolid);

		Novice::DrawSprite((int)player.position.x, (int)player.position.y, genjiGraphHandle, 1, 1, 0, WHITE);
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
