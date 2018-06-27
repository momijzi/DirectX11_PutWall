#include"App.hpp"

GameScene::GameScene()
{
				App::Initialize();

				//�}�E�X���g������������ꍇ�͏��������ŏ��ɂ��Ă������Ƃ𐄏�����
				App::SetMousePosition(0.0f, 0.0f);

				mainCamera.position = Float3(0.0f, 10.0f, -10.0f);
				currentTurn = downLimit;

				bgm_title.Load(L"music/BGM/bgm_title.mp3");
				bgm_main.Load(L"music/BGM/bgm_main.mp3");

				se_ok.Load("music/SE/se_ok.wav");
				se_cancel.Load("music/SE/se_cancel.wav");
				se_select.Load("music/SE/se_select.wav");
				se_move.Load("music/SE/se_move.wav");
				se_stop.Load("music/SE/se_stop.wav");

				bgm.Play(bgm_title);
}

void GameScene::SceneManager()
{
				bgm.UpDate();
				switch (gameState)
				{
								case GameState::PLAY:
												
												CameraAnglesChangeMouse();
												CameraPositionMove();
												MainTurn();
												mainCamera.Update(true);
												pMana.Draw(wall.MaxLength, wall.blockSize,downTimeCount / downTime);
												wall.Draw(pMana.GetDrawFlag(), downTimeCount / downTime, Float2(pMana.GetPosition(true).y, pMana.GetPosition(false).y));
												break;
								default:
												//��ɕ`�悵���ύX���Ȃ��̂ňꊇ�ŕύX����
												if (App::GetKeyDown(VK_RETURN))
												{
																if (gameState == TITLE)
																{
																				bgm.Play(bgm_main);
																				uiData.CreateStateUi(Float2(0.0f, 1.0f));
																				gameState = PLAY;
																}
																else if (gameState == OVER)
																{
																				uiData.CreateStateUi(Float2(0.0f, 0.0f));
																				Release();
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
				uiData.Draw(pMana.GetPushLength(),currentTurn);
}

//�ړ��@���@�����o��
//�����o���@���@�ړ�
//�ǂ��炪�헪���オ��񂾂낤���c�c
void GameScene::MainTurn()
{
				switch (scene)
				{
								case GameScene::TURN_FIRST:
												//�����ɃA�j���[�V�����i��U�̃^�[���݂����ȁj����ꂽ����

												//���̃v���C���[���ړ��ł���ꏊ���i�[
												//�����ɃA�j���[�V�������I���������scene���ς��悤�ɂ���flag������
												if (true)
												{
																pMana.NextTurn();

																wall.wallData.ResetWallData();
																wall.SelectLookWall(pMana.GetPosition(true).y - 1, mainCamera.angles.y);
																testDirection = 0;
																wall.SelectToWall(testDirection,Float2(pMana.GetPosition(true).y, pMana.GetPosition(false).y));
																scene = PUSH_WALL_SELECT;
												}
												break;
								case GameScene::PUSH_WALL_SELECT:
												//�����o���ǂ�I������@�\���őI���ɂȂ�Ǝv����
												//�I������Ƃ���y�������ړ��̒��S����ǂ��m�F����
												testDirection = KeyMoveData() + 1;
												if (testDirection != 0)
												{
																if (wall.SelectToWall(testDirection, Float2(pMana.GetPosition(true).y, pMana.GetPosition(false).y)))
																{
																				//�w�肵�������ɍs�����Ƃ��\
																				se.Play(se_select);
																}
																else
																{
																				//�w�肵�������ɍs���Ȃ�
																				se.Play(se_cancel);
																}
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																if (wall.wallData.drawTexFlag == 2)
																{
																				se.Play(se_ok);
																				//�I�������ꏊ�ŉ����o�����Ƃ��\�Ȃ��Ƃ��킩��܂���
																				wall.wallData.drawTexFlag = 0;
																				wall.wallData.checkLengthFlag = true;
																				wall.wallData.length = 1;
																				scene = SET_PUSH_WALL_LENGTH;
																}
																else
																{
																				//�I�������ꏊ�����łɉ����o����Ă����ӏ��ł�����
																				se.Play(se_cancel);	
																}
												}
												break;
								case GameScene::SET_PUSH_WALL_LENGTH:
												//�����o���������܂����߂Ă��Ȃ�
												if (App::GetKeyDown(VK_UP))
												{
																if (wall.wallData.length != pMana.GetCurrentPlayerPushLength())
																{
																				se.Play(se_select);
																				wall.SetPushWallLength(1, pMana.GetPosition(true),
																								pMana.GetPosition(false), pMana.GetTurn());
																}
												}
												else if (App::GetKeyDown(VK_DOWN))
												{
																se.Play(se_select);
																wall.SetPushWallLength(-1);
												}
												else if (App::GetKeyDown(VK_LEFT))
												{
																se.Play(se_select);
																wall.ChangePushWallLine();
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																//�����������������܂�܂����̂Ŋm�F���s���܂�
																//�����͒Z���V�[���ɂȂ�̂Ńt���O�Ǘ��ł����Ǝv���܂�
																wall.wallData.drawTexFlag = false;
																wall.wallData.checkLengthFlag = false;
																wall.wallData.moveFlag = true;

																pMana.DeliverLength(wall.wallData.length);
																scene = PUSH_WALL;
												}
												else if (App::GetKeyDown(VK_ESCAPE))
												{
																se.Play(se_cancel);
																wall.wallData.drawTexFlag = 2;
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

																wall.ResetPlayerMoveFlag();
																wall.SetPlayerMoveFlag(pMana.GetPosition(true));
																pMana.MovementRange(&wall);
																pMana.SetDrawFlag(true);

																scene = PLAYER_MOVE;
												}
												break;
								case GameScene::PLAYER_MOVE:
												//�\���L�[�ňړ��Ƃ���ړ������̓��͔͂z�񏇎�Ƃ���
												//�J�����̃A���O���ŕ�����ς���ׂ���
								{
												int moveDirection = KeyMoveData();

												if (moveDirection != -1)
												{
																se.Play(se_select);
																pMana.MoveableChack(&wall, moveDirection);
												}

												if (App::GetKeyDown(VK_RETURN))
												{
																if (pMana.MoveFlagChack())
																{
																				se.Play(se_ok);
																				
																				if (pMana.GetPosition(true).y == wall.MaxHeight)
																				{
																								uiData.CreateStateUi(Float2(1.0f, 1.0f));
																								gameState = OVER;
																								break;
																				}

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
																//�c��o�ߎ��Ԃ̕`�������
																scene = TURN_FIRST;
												}
												else
												{
																scene = DROP_ANIMATION;
												}
												break;
								case GameScene::DROP_ANIMATION:
												downTimeCount += App::GetDeltaTime();
												//dropTime�̃X�s�[�h�ŗ��Ƃ�
												if (!(downTimeCount < downTime))
												{
																downTimeCount = 0.0f;
																currentTurn = downLimit;
																wall.DownData();
																if (pMana.DownPlayer())
																{
																				uiData.CreateStateUi(Float2(1.0f, 1.0f));
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
												0.0f)*0.1f;
								move.angles += Float3(
								-App::GetMousePosition().y,
								App::GetMousePosition().x,
								0.0f)*0.1f;

								App::SetMousePosition(0.0f, 0.0f);
				}
}

//���̃f�[�^�̓J�����̈ړ��Ȃ̂ł��Ƃ�CameraMove�Ɉڂ��ׂ�
void GameScene::CameraPositionMove()
{
				bool posUpdate = false;
				//�J�����̈ړ�

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

//�ړ��L�[���������Ƃ��Ƀf�[�^��Ԃ�
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
//������
void GameScene::Release()
{
				currentTurn = downLimit;
				pMana.Release();
				wall.Release();
}