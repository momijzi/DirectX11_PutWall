#include"App.hpp"

GameScene::GameScene()
{
				App::Initialize();

				//マウスを使う処理がある場合は初期化を最初にしておくことを推奨する
				App::SetMousePosition(0.0f, 0.0f);

				mainCamera.position = Float3(0.0f, 10.0f, -10.0f);
				currentTurn = downLimit;

				bgm_title.Load(L"music/BGM/bgm_title.mp3");
				bgm_main.Load(L"music/BGM/bgm_main.mp3");
				bgm_end.Load(L"music/BGM/bgm_end.mp3");

				se_ok.Load("music/SE/se_ok.wav");
				se_cancel.Load("music/SE/se_cancel.wav");
				se_select.Load("music/SE/se_select.wav");
				se_move.Load("music/SE/se_move.wav");
				se_stop.Load("music/SE/se_stop.wav");
				se_cutin.Load("music/SE/se_cutin.wav");

				bgm.Play(bgm_title);
}

void GameScene::SceneManager()
{
				bgm.UpDate();
				CameraAnglesChangeMouse();
				CameraPositionMove();
				
				switch (gameState)
				{
								case GameState::PLAY:
												MainTurn();
												mainCamera.Update(true);
												ray.Draw();
												pMana.Draw(wall.MaxLength, wall.blockSize,downTimeCount / (float)downTime);
												wall.Draw(pMana.GetDrawFlag(), downTimeCount / downTime, Float2(pMana.GetPosition(true).y, pMana.GetPosition(false).y),pMana.GetTurn());
												break;
								default:
												//主に描画しか変更がないので一括で変更する
												if (App::GetKeyDown(VK_RETURN))
												{
																if (gameState == TITLE)
																{
																				bgm.Play(bgm_main);
																				Release();
																				gameState = PLAY;
																}
																else if (gameState == OVER)
																{
																				bgm.Play(bgm_title);
																				gameState = TITLE;
																}
												}
												break;
				}
				uiData.Draw((int)gameState,pMana.GetPushLength(),currentTurn,pMana.GetTurn());
}

//移動　→　押し出し
//押し出し　→　移動
//どちらが戦略性上がるんだろうか……
void GameScene::MainTurn()
{
				switch (scene)
				{
								case GameScene::TURN_FIRST:
												//カットインの音～
												if (uiData.GetCountTime() == 0)
												{
																se.Play(se_cutin);
												}
												//次のプレイヤーが移動できる場所を格納
												//ここにアニメーションが終わった時にsceneが変わるようにするflagを入れる
												if (uiData.Cutin(pMana.GetTurn()))
												{
																pMana.PlusPushLength(wall.MaxLength);
																wall.wallData.ResetWallData();
																wall.SelectLookWall(pMana.GetPosition(true).y - 1, mainCamera.angles.y);
																testDirection = 0;
																wall.SelectToWall(testDirection, pMana.GetPosition(true), pMana.GetPosition(false));
																scene = PUSH_WALL_SELECT;
												}
												break;
								case GameScene::PUSH_WALL_SELECT:
												//押し出す壁を選択する　十字で選択になると思われる
												//選択するときはy軸だけ移動の中心から壁を確認する
												testDirection = KeyMoveData() + 1;
												if (testDirection != 0)
												{
																wall.SelectToWall(testDirection, pMana.GetPosition(true), pMana.GetPosition(false));
																if (wall.wallData.drawTexFlag)
																{
																				//指定した方向に行くことが可能
																				se.Play(se_select);
																}
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																if (wall.wallData.drawTexFlag)
																{
																				se.Play(se_ok);
																				//選択した場所で押し出すことが可能なことがわかりました
																				wall.wallData.drawTexFlag = false;
																				wall.wallData.checkLengthFlag = true;
																				wall.wallData.length = 1;
																				scene = SET_PUSH_WALL_LENGTH;
																}
																else
																{
																				//選択した場所がすでに押し出されていた箇所であった
																				se.Play(se_cancel);	
																}
												}
												ray.UpDate(mainCamera.position, wall.wallData.createInitPosition * wall.blockSize);
												break;
								case GameScene::SET_PUSH_WALL_LENGTH:
												//押し出す長さを変えます UPは,伸ばす、DOWNは,戻す
												if (App::GetKeyDown(VK_UP))
												{
																if (wall.wallData.length != pMana.GetCurrentTurnPushLength())
																{
																				//押し戻せるかの処理すでに戻せる限界にあるかどうかのチェック
																				if (wall.SetPushWallLength(1, pMana.GetPosition(true),
																								pMana.GetPosition(false), pMana.GetTurn()))
																				{
																								se.Play(se_select);
																				}
																				else
																				{
																								se.Play(se_cancel);
																				}
																}
												}
												else if (App::GetKeyDown(VK_DOWN))
												{
																//押し戻せるかの処理すでに戻せる限界にあるかどうかのチェック
																if (wall.SetPushWallLength(-1))
																{
																				se.Play(se_select);
																}
																else
																{
																				se.Play(se_cancel);
																}
												}
												//特殊ブロックの配置セット
												//一方向にしているのは二方向処理して混乱しないようにするため
												else if (App::GetKeyDown(VK_LEFT))
												{
																se.Play(se_select);
																wall.ChangePushWallLine();
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																//押しだす長さも決まりましたので確認を行います
																//ここは短いシーンになるのでフラグ管理でいいと思います
																wall.wallData.checkLengthFlag = false;
																wall.wallData.moveFlag = true;

																scene = PUSH_WALL;
												}
												else if (App::GetKeyDown(VK_ESCAPE))
												{
																se.Play(se_cancel);
																wall.wallData.drawTexFlag = true;
																wall.wallData.checkLengthFlag = false;
																wall.wallData.PushTypeReset();
																scene = PUSH_WALL_SELECT;
												}
												break;
								case GameScene::PUSH_WALL:
												wall.MoveWall();
												if (!wall.wallData.moveFlag)
												{
																se.Play(se_stop);

																//出した分だけ押し出せる長さを引く
																pMana.SetPushLength(wall.wallData.length);

																//プレイヤーの移動場所の設定
																wall.ResetPlayerMoveFlag();
																wall.SetPlayerMoveFlag(pMana.GetPosition(true));
																pMana.MovementRange(&wall);
																pMana.SetDrawFlag(true);
																

																scene = PLAYER_MOVE;
												}
												break;
								case GameScene::PLAYER_MOVE:
												//十字キーで移動とする移動方向の入力は配列順守とする
												//カメラのアングルで方向を変えるべきか
								{
												int moveDirection = KeyMoveData();

												if (moveDirection != -1)
												{
																se.Play(se_select);
																//ここで現在のカメラの角度からみてプレイヤーの移動をしたいので移動方向を直す
																for (int i = 1; i <= 4; i++)
																{
																				if (mainCamera.angles.y - (i * 90.0f) < -45.0f)
																				{
																								moveDirection -= i - 1;
																								if (moveDirection < 0)
																								{
																												//0以下の時0～4に戻す必要があるため3に戻す
																												moveDirection += 4;
																								}
																								break;
																				}
																}
																pMana.MoveableChack(&wall, moveDirection);
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																if (pMana.MoveFlagChack())
																{
																				se.Play(se_ok);
																				
																				if (pMana.GetPosition(true).y == wall.MaxHeight)
																				{
																								bgm.Play(bgm_end);
																								gameState = OVER;
																								break;
																				}
																				//次のターンに変更
																				pMana.NextTurn();
																				pMana.SetDrawFlag(false);
																				scene = TURN_FIRST;
																				if (!pMana.GetTurn())
																				{
																								currentTurn--;
																								scene = DROP_CHACK;
																				}
																}
																else
																{
																				se.Play(se_cancel);
																}
												}
								}
								break;
								case GameScene::DROP_CHACK:
												if (currentTurn != 0)
												{
																//残り経過時間の描画をする
																scene = TURN_FIRST;
												}
												else
												{
																scene = DROP_ANIMATION;
												}
												break;
								case GameScene::DROP_ANIMATION:
												downTimeCount += App::GetDeltaTime();
												//dropTimeのスピードで落とす
												if (!(downTimeCount < downTime))
												{
																//落下が終わった
																se.Play(se_stop);

																downTimeCount = 0.0f;
																currentTurn = downLimit;
																wall.DownData();
																if (pMana.DownPlayer())
																{
																				bgm.Play(bgm_end);
																				gameState = OVER;
																}
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
												0.0f)*angleSpeed;
								
								//回転で315を超えた時-45.0に戻す、逆もしかり
								//中途半端な数値の理由は、プレイヤーなどの移動方向を調整するときに-45.0されている数値の方が都合がいいから
								if (mainCamera.angles.y < -45.0)
								{
												mainCamera.angles.y = 315.0f;
								}
								else if (mainCamera.angles.y > 315.0f)
								{
												mainCamera.angles.y = -45.0f;
								}
								//カメラのアングルを制限
								if (mainCamera.angles.x < -45.0f)
								{
												mainCamera.angles.x = -45.0f;
								}
								else if (mainCamera.angles.x > 60.0f)
								{
												mainCamera.angles.x = 60.0f;
								}
								App::SetMousePosition(0.0f, 0.0f);
				}
}

//このデータはカメラの移動なのであとでCameraMoveに移すべし
void GameScene::CameraPositionMove()
{
				//カメラの移動があったかどうかの判定用
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
												mainCamera.position = move.MovePos(mainCamera.position, mainCamera.angles,moveSpeed,true, false, false, true);
												posUpdate = true;
								}
								else if (App::GetKey('S'))
								{
												mainCamera.position = move.MovePos(mainCamera.position, mainCamera.angles, moveSpeed, false, false, false, true);
												posUpdate = true;
								}

								if (App::GetKey('D'))
								{
												mainCamera.position = move.MovePos(mainCamera.position, mainCamera.angles, moveSpeed, true, true, false, false);
												posUpdate = true;
								}
								else if (App::GetKey('A'))
								{
												mainCamera.position = move.MovePos(mainCamera.position, mainCamera.angles, moveSpeed, false, true, false, false);
												posUpdate = true;
								}
				}
				//カメラの移動を確認したら移動が制限以上の場所に行っていないかを判定
				if (posUpdate)
				{
								if (fabs(mainCamera.position.x) > wall.MaxLength * 2)
								{
												if (mainCamera.position.x > 0)
												{
																mainCamera.position.x = (float)(wall.MaxLength * 2);
												}
												else
												{
																mainCamera.position.x = -(float)(wall.MaxLength * 2);
												}
								}
								if (mainCamera.position.y < 0.5f * wall.blockSize || mainCamera.position.y >(wall.MaxHeight + 3 + 0.5f) * wall.blockSize)
								{
												if (mainCamera.position.y < 0.5f * wall.blockSize)
												{
																mainCamera.position.y = 0.5f * wall.blockSize;
												}
												else
												{
																mainCamera.position.y = (float)((wall.MaxHeight + 3 + 0.5f) * wall.blockSize);
												}
								}
								if (fabs(mainCamera.position.z) > wall.MaxLength * 2)
								{
												if (mainCamera.position.z > 0)
												{
																mainCamera.position.z = (float)(wall.MaxLength * 2);
												}
												else
												{
																mainCamera.position.z = -(float)(wall.MaxLength * 2);
												}
								}
				}
}

//移動キーを押したときに方向に応じてデータを返す
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
//初期化
void GameScene::Release()
{
				currentTurn = downLimit;
				pMana.Release((float)wall.MaxLength);
				wall.Release();
}