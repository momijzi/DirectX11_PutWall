#pragma once
class GameScene
{
public:
				GameScene();
				~GameScene() {};

				void SceneManager();

				void MainTurn();

				//カメラの角度をマウスで変更する
				//これはカメラマネージャーで書くべきだが今回は妥協
				void CameraAnglesChangeMouse();
				//カメラの座標移動
				void CameraPositionMove();
				//移動しているときその移動はどの方向に行われているかを確認する
				int KeyMoveData();
				//初期化
				void Release();
private:
				//ゲームの流れ
				enum GameState { TITLE, CONFIG, PLAY, OVER };
				GameState gameState = TITLE;

				//プレイ中の基本的な流れ
				enum Scene { TURN_FIRST, PUSH_WALL_SELECT, SET_PUSH_WALL_LENGTH,
								PUSH_WALL, PLAYER_MOVE,TURN_END, DROP_CHACK, DROP_ANIMATION};
				Scene scene = TURN_FIRST;

				//カメラ生成
				//3D空間を描画に使用するカメラ
				Camera mainCamera;
				//壁と中のブロックを主に描画する
				Wall wall;
				//プレイヤーの移動などの処理を行う
				PlayerManager pMana;
				//UIを描画するときに使用する　ただし中身で宣言したUI描画するだけ
				UIData uiData;
				//カメラの移動を行う（完全ではないｙ軸が明らかおかしいので）
				Move move;
				//Wallの押し出しを行う処理の試験用に使用
				int testDirection = 0;
				//残り何ターンで沈むか
				const int downLimit = 3;
				//経過ターン
				int currentTurn = 0;
				//沈む速度の設定
				const int downTime = 2;
				//落ちてからの時間経過を保持する
				float downTimeCount = 0;

				MediaFoundation bgm;
				XAudio2 se;

				MediaFoundation::AudioData bgm_title;
				MediaFoundation::AudioData bgm_main;

				XAudio2::WaveData se_ok;
				XAudio2::WaveData se_select;
				XAudio2::WaveData se_cancel;
				XAudio2::WaveData se_move;
				XAudio2::WaveData se_stop;
};