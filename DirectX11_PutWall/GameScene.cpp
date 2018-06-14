#include"App.hpp"
GameScene::GameScene()
{
				App::Initialize();

				//マウスを使う処理がある場合は初期化を最初にしておくことを推奨する
				App::SetMousePosition(0.0f, 0.0f);

				mainCamera.position = Float3(0.0f, 5.0f, -10.0f);
}

void GameScene::SceneManager()
{
				mainCamera.Update();
				switch (gameState)
				{
								case PLAY:
												MainTurn();
												if (App::GetKeyDown(VK_RETURN))//ほんとはここにエンドフラグを//てかここはMainTurnに書かれるべき
												{
																uiData.stateUi.Create(Float2(1.0f, 1.0f));
																gameState = OVER;
												}
												wall.Draw();
												break;
								default:

												//主に描画しか変更がないので一括で変更する
												if (App::GetKeyDown(VK_RETURN))
												{
																if (gameState == TITLE)
																{
																				uiData.stateUi.Create(Float2(0.0f, 1.0f));
																				gameState = PLAY;
																}
																else if (gameState == OVER)
																{
																				uiData.stateUi.Create(Float2(0.0f, 0.0f));
																				gameState = TITLE;
																}
												}
												else if (App::GetKeyDown(VK_SPACE))
												{
																if (gameState != CONFIG)
																{
																				uiData.stateUi.Create(Float2(1.0f, 0.0f));
																				gameState = CONFIG;
																}
																else
																{
																				uiData.stateUi.Create(Float2(0.0f, 0.0f));
																				gameState = TITLE;
																}
												}
												break;
				}
				uiData.Draw();
}

void GameScene::MainTurn()
{
				switch (scene)
				{
								case GameScene::TURN_FIRST:
												//次のプレイヤーが移動できる場所を格納
												wall.ResetPlayerMoveFlag();
												playerManager.NewTurn();
												playerManager.MovementRange(&wall);

												//ここにアニメーション（先攻のターンみたいな）を入れたい所
												break;
								case GameScene::PLAYER_MOVE:
												//十字キーで移動とする移動方向の入力は配列順守とする

												break;
								case GameScene::PUSH_WALL_SELECT:
												//押し出す壁を選択する　十字で選択になると思われる

												break;
								case GameScene::SET_PUSH_WALL_LENGTH:

												break;
								case GameScene::MOVE_CHACK:
												//カメラを動かして周りを確認する

												break;
								case GameScene::TURN_END:
												//ターンエンドの宣言をする場所　ここでfalseを返せばMoveChackに移動
								{
												//引数に選択している情報を入れればおけ
												Wall::WallData wallData(testSurface, testWidth, testHeight, testLength, testCount);
												wall.SetInitialPosition(wallData);
								}
								break;
				}
}

void GameScene::CameraAnglesChangeMouse()
{
				if (App::GetKey(VK_LBUTTON))
				{
								if (App::GetKeyDown(VK_LBUTTON))
								{
												App::SetMousePosition(0.0f, 0.0f);
								}
								mainCamera.angles += Float3(
												-App::GetMousePosition().y,
												App::GetMousePosition().x,
												0.0f)*0.1f;
								/*move.angles += Float3(
								-App::GetMousePosition().y,
								App::GetMousePosition().x,
								0.0f)*0.1f;*/

								App::SetMousePosition(0.0f, 0.0f);
				}
}

void GameScene::CameraPositionMove()
{
				//プレイヤーの移動
				if (App::GetKey(VK_UP))
				{
								mainCamera.position = move.MovePos(mainCamera.position, true, false, true, true);
				}
				else if (App::GetKey(VK_DOWN))
				{
								mainCamera.position = move.MovePos(mainCamera.position, false, false, true, true);
				}
				if (App::GetKey(VK_RIGHT))
				{
								mainCamera.position = move.MovePos(mainCamera.position, true, true, true, false);
				}
				else if (App::GetKey(VK_LEFT))
				{
								mainCamera.position = move.MovePos(mainCamera.position, false, true, true, false);
				}
}

void GameScene::Release()
{
				playerManager.Release();
				wall.Release();
}