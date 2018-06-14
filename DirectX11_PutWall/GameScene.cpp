#include"App.hpp"
GameScene::GameScene()
{
				App::Initialize();

				//マウスを使う処理がある場合は初期化を最初にしておくことを推奨する
				App::SetMousePosition(0.0f, 0.0f);

				mainCamera.position = Float3(0.0f, 10.0f, 0.0f);
}

void GameScene::SceneManager()
{
				CameraAnglesChangeMouse();
				mainCamera.Update();
				switch (gameState)
				{
								case GameState::PLAY:
												MainTurn();
												//if (App::GetKeyDown(VK_RETURN))//ほんとはここにエンドフラグを//てかここはMainTurnに書かれるべき
												//{
												//				uiData.CreateStateUi(Float2(1.0f, 1.0f));
												//				gameState = OVER;
												//}
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
				uiData.Draw();
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
																				wall.SelectLookWall(testHeight, mainCamera.angles.y, testSurface, testWidth, testFlag);
																				scene = PUSH_WALL_SELECT;
																}
																//まだ移動を完了してないです。みたいなの欲しいな〜
												}
								}
												break;
								case GameScene::PUSH_WALL_SELECT:
												//押し出す壁を選択する　十字で選択になると思われる
												//選択するときはy軸だけ移動の中心から壁を確認する
												testDirection = KeyMoveData() + 1;
												if (testDirection != 0)
												{
																wall.SelectToWall(testSurface, testHeight, testWidth, testDirection, testWallFlag);
												}
												
												if (App::GetKeyDown(VK_RETURN))
												{
																if (testWallFlag)
																{
																				//選択した場所で押し出すことが可能なことがわかりました
																				scene = SET_PUSH_WALL_LENGTH;
																}
																else
																{
																				//選択した場所がすでに押し出されていた箇所であった
																				//ぶっぶー的な？
																}
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																//scene = SET_PUSH_WALL_LENGTH;
												}

												if (App::GetKeyDown(VK_ESCAPE))
												{
																playerManager.SetPlayerDrawFlag(true);
																playerManager.ReturnMovePos();
																scene = PLAYER_MOVE;
												}
												break;
								case GameScene::SET_PUSH_WALL_LENGTH:
												if (true)
												{
																//押し出す長さをまだ決めていない
																if (true)//なにかで壁から押し出すブロックの長さを決める
																{
																				if (App::GetKeyDown(VK_UP))
																				{
																								if (!wall.GetBlockData(testWidth >> 1, (testLength) >> 1, testHeight >> 1))
																								{
																												testLength++;
																								}
																				}
																				else if (App::GetKeyDown(VK_DOWN))
																				{
																								if (testLength > 1)
																								{
																												testLength--;
																								}
																				}
																}

																if (App::GetKeyDown(VK_RETURN))
																{
																				//押しだす長さも決まりましたので確認を行います
																				//ここは短いシーンになるのでフラグ管理でいいと思います

																				//検証用、後で消すこと_久保田
																				wallData.SetWallData(testSurface, testWidth, testHeight, testLength, testCount);
																				wall.SetInitialPosition(wallData);
																				scene = TURN_END;
																}
												}
												else
												{
																//押し出す長さを決定する
																//完全な決定
																//ここはおそらくENTERだと誤って押してしまう可能性があるため注意
																if (App::GetKeyDown(VK_RETURN))
																{
																				scene = TURN_END;
																}
												}
												break;
								case GameScene::CAMERA_MOVE_CHACK:
												//カメラを動かして周りを確認する
												if (KeyMoveData() != -1)
												{
																//移動したことがわかる
																//今回は箱の中しか移動しない予定なので
																//制限はしっかりと
												}
												break;
								case GameScene::TURN_END:
												//ターンエンドの宣言をする場所　ここでfalseを返せばMoveChackに移動
								{
												//引数に選択している情報を入れればおけ
												wall.MoveWall(wallData);
								}

								if (false)//押し出しの処理が終わったかどうか
								{
												
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

void GameScene::CameraPositionMove()
{
				//カメラの移動
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