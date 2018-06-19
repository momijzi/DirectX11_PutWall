#include"App.hpp"

//Camera をLookPosで統一してみてもいいかも？

//現在のバグ
//プレイヤーの移動をしてから選択に移動するときにバグが発生する
//なぜか不明
//ただ配列外に行くことが悪いことはわかった


GameScene::GameScene()
{
				App::Initialize();

				//マウスを使う処理がある場合は初期化を最初にしておくことを推奨する
				App::SetMousePosition(0.0f, 0.0f);

				mainCamera.position = Float3(0.0f, 10.0f, 0.0f);
}

void GameScene::SceneManager()
{
				switch (gameState)
				{
								case GameState::PLAY:
												
												CameraAnglesChangeMouse();
												CameraPositionMove();
												
												MainTurn();
												//if (App::GetKeyDown(VK_RETURN))//ほんとはここにエンドフラグを//てかここはMainTurnに書かれるべき
												//{
												//				uiData.CreateStateUi(Float2(1.0f, 1.0f));
												//				gameState = OVER;
												//}
												mainCamera.Update(true);
												playerManager.Draw(wall.length, wall.blockSize);
												wall.Draw(playerManager.GetPlayerDrawFlag());
												break;
								default:
												//主に描画しか変更がないので一括で変更する
												if (App::GetKeyDown(VK_RETURN))
												{
																if (gameState == TITLE)
																{
																				uiData.CreateStateUi(Float2(0.0f, 1.0f));
																				gameState = PLAY;
																}
																else if (gameState == OVER)
																{
																				uiData.CreateStateUi(Float2(0.0f, 0.0f));
																				gameState = TITLE;
																}
												}
												else if (App::GetKeyDown(VK_SPACE))
												{
																if (gameState != CONFIG)
																{
																				uiData.CreateStateUi(Float2(1.0f, 0.0f));
																				gameState = CONFIG;
																}
																else
																{
																				uiData.CreateStateUi(Float2(0.0f, 0.0f));
																				gameState = TITLE;
																}
												}
												break;
				}
				uiData.Draw(playerManager.GetPlayerPushLength());
}

void GameScene::MainTurn()
{
				switch (scene)
				{
								case GameScene::TURN_FIRST:
												//ここにアニメーション（先攻のターンみたいな）を入れたい所

												//次のプレイヤーが移動できる場所を格納
												//ここにアニメーションが終わった時にsceneが変わるようにするflagを入れる
												if (true)
												{
																wall.ResetPlayerMoveFlag();
																playerManager.NextTurn();
																playerManager.MovementRange(&wall);
																playerManager.SetPlayerDrawFlag(true);
																scene = PLAYER_MOVE;
												}
												break;
								case GameScene::PLAYER_MOVE:
												//十字キーで移動とする移動方向の入力は配列順守とする
								{
												int moveDirection = KeyMoveData();

												if (moveDirection != -1)
												{
																playerManager.MoveableChack(&wall, moveDirection);
												}

												if (App::GetKeyDown(VK_RETURN))
												{
																if (playerManager.MoveFlagChack())
																{
																				playerManager.SetPlayerDrawFlag(false);

																				wall.wallData.SetWallData();
																				wall.wallData.length = 1;
																				wall.wallData.drawTexFlag = true;

																				wall.SelectLookWall(4.0f, mainCamera.angles.y);
																				testDirection = 0;
																				wall.SelectToWall(testDirection);

																				scene = PUSH_WALL_SELECT;
																}
																//まだ移動を完了してないです。みたいなの欲しいな～
												}
								}
								break;
								case GameScene::PUSH_WALL_SELECT:
												//押し出す壁を選択する　十字で選択になると思われる
												//選択するときはy軸だけ移動の中心から壁を確認する
												testDirection = KeyMoveData() + 1;
												if (testDirection != 0)
												{
																wall.SelectToWall(testDirection);
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																if (wall.wallData.drawTexFlag)
																{
																				//選択した場所で押し出すことが可能なことがわかりました
																				wall.SetInitialPosition();
																				wall.wallData.checkLengthFlag = true;
																				scene = SET_PUSH_WALL_LENGTH;
																}
																else
																{
																				//選択した場所がすでに押し出されていた箇所であった
																				//ぶっぶー的な？
																}
												}
												if (App::GetKeyDown(VK_ESCAPE))
												{
																playerManager.SetPlayerDrawFlag(true);
																playerManager.ReturnMovePos();
																scene = PLAYER_MOVE;
												}
												break;
								case GameScene::SET_PUSH_WALL_LENGTH:
												//押し出す長さをまだ決めていない

												if (App::GetKeyDown(VK_UP))
												{
																if (wall.wallData.length != playerManager.GetCurrentPlayerPushLength())
																{
																				wall.wallData.length++;
																}
												}
												else if (App::GetKeyDown(VK_DOWN))
												{
																if (wall.wallData.length > 1)
																{
																				wall.wallData.length--;
																}
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																//押しだす長さも決まりましたので確認を行います
																//ここは短いシーンになるのでフラグ管理でいいと思います

																wall.wallData.checkLengthFlag = false;
																wall.wallData.moveFlag = true;
																
																playerManager.DeliverLength(wall.wallData.length);
																scene = TURN_END;
												}

												break;
								case GameScene::TURN_END:
												//ターンエンドの宣言をする場所　ここでfalseを返せばMoveChackに移動
								
												//引数に選択している情報を入れればおけ
												wall.MoveWall();
												if (!wall.wallData.moveFlag)
												{
																scene = TURN_FIRST;
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
								move.angles += Float3(
								-App::GetMousePosition().y,
								App::GetMousePosition().x,
								0.0f)*0.1f;

								App::SetMousePosition(0.0f, 0.0f);
				}
}

//このデータはカメラの移動なのであとでCameraMoveに移すべし
void GameScene::CameraPositionMove()
{
				bool posUpdate = false;
				//カメラの移動

				if (App::GetKey(VK_SHIFT))
				{
								if (App::GetKey('W'))
								{
												mainCamera.position.y+=0.1f;
								}
								else if (App::GetKey('S'))
								{
												mainCamera.position.y-=0.1f;
								}
								posUpdate = true;
				}
				else
				{
								if (App::GetKey('W'))
								{
												mainCamera.position = move.MovePos(mainCamera.position, true, false, false, true);
												posUpdate = true;
								}
								else if (App::GetKey('S'))
								{
												mainCamera.position = move.MovePos(mainCamera.position, false, false, false, true);
												posUpdate = true;
								}

								if (App::GetKey('D'))
								{
												mainCamera.position = move.MovePos(mainCamera.position, true, true, false, false);
												posUpdate = true;
								}
								else if (App::GetKey('A'))
								{
												mainCamera.position = move.MovePos(mainCamera.position, false, true, false, false);
												posUpdate = true;
								}
				}
				if (posUpdate)
				{
								if (fabs(mainCamera.position.x) > wall.length)
								{
												if (mainCamera.position.x > 0)
												{
																mainCamera.position.x = (float)(wall.length);
												}
												else
												{
																mainCamera.position.x = -(float)(wall.length);
												}
								}
								if (mainCamera.position.y < 0.5f * wall.blockSize || mainCamera.position.y >(wall.height + 3 + 0.5f) * wall.blockSize)
								{
												if (mainCamera.position.y < 0.5f * wall.blockSize)
												{
																mainCamera.position.y = 0.5f * wall.blockSize;
												}
												else
												{
																mainCamera.position.y = (float)((wall.height + 3 + 0.5f) * wall.blockSize);
												}
								}

								if (fabs(mainCamera.position.z) > wall.length)
								{
												if (mainCamera.position.z > 0)
												{
																mainCamera.position.z = (float)(wall.length);
												}
												else
												{
																mainCamera.position.z = -(float)(wall.length);
												}
								}
				}
}

int GameScene::KeyMoveData()
{
				if (App::GetKeyDown(VK_UP))
				{
								return 0;
				}
				else if (App::GetKeyDown(VK_LEFT))
				{
								return 1;
				}
				else if (App::GetKeyDown(VK_DOWN))
				{
								return 2;
				}
				if (App::GetKeyDown(VK_RIGHT))
				{
								return 3;
				}
				return -1;
}

void GameScene::Release()
{
				playerManager.Release();
				wall.Release();
}