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
				
				void CameraPositionMove();
								
				void Release();

private:
				//ゲームの流れ
				enum GameState { TITLE, CONFIG, PLAY, OVER };
				GameState gameState = TITLE;

				//プレイ中の基本的な流れ
				enum Scene { TURN_FIRST, PLAYER_MOVE, PUSH_WALL_SELECT, SET_PUSH_WALL_LENGTH, MOVE_CHACK, TURN_END };
				Scene scene = TURN_FIRST;

				//カメラ生成
				//3D空間を描画に使用するカメラ
				Camera mainCamera;

				Wall wall;
				PlayerManager playerManager;
				UIData uiData;
				Move move;

				//Wallの押し出しを行う処理の試験用に使用
				int testSurface = 0;
				int testWidth = 3;
				int testHeight = 4;
				int testLength = 5;
				Float3 testPos[5] = {};
				bool testFlag = false;
				float testCount = 0.0f;

				
};