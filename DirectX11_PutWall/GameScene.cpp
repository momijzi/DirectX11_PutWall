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
												//��ɕ`�悵���ύX���Ȃ��̂ňꊇ�ŕύX����
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

//�ړ��@���@�����o��
//�����o���@���@�ړ�
//�ǂ��炪�헪���オ��񂾂낤���c�c
void GameScene::MainTurn()
{
				switch (scene)
				{
								case GameScene::TURN_FIRST:
												//�J�b�g�C���̉��`
												if (uiData.GetCountTime() == 0)
												{
																se.Play(se_cutin);
												}
												//���̃v���C���[���ړ��ł���ꏊ���i�[
												//�����ɃA�j���[�V�������I���������scene���ς��悤�ɂ���flag������
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
												//�����o���ǂ�I������@�\���őI���ɂȂ�Ǝv����
												//�I������Ƃ���y�������ړ��̒��S����ǂ��m�F����
												testDirection = KeyMoveData() + 1;
												if (testDirection != 0)
												{
																wall.SelectToWall(testDirection, pMana.GetPosition(true), pMana.GetPosition(false));
																if (wall.wallData.drawTexFlag)
																{
																				//�w�肵�������ɍs�����Ƃ��\
																				se.Play(se_select);
																}
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																if (wall.wallData.drawTexFlag)
																{
																				se.Play(se_ok);
																				//�I�������ꏊ�ŉ����o�����Ƃ��\�Ȃ��Ƃ��킩��܂���
																				wall.wallData.drawTexFlag = false;
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
												ray.UpDate(mainCamera.position, wall.wallData.createInitPosition * wall.blockSize);
												break;
								case GameScene::SET_PUSH_WALL_LENGTH:
												//�����o��������ς��܂� UP��,�L�΂��ADOWN��,�߂�
												if (App::GetKeyDown(VK_UP))
												{
																if (wall.wallData.length != pMana.GetCurrentTurnPushLength())
																{
																				//�����߂��邩�̏������łɖ߂�����E�ɂ��邩�ǂ����̃`�F�b�N
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
																//�����߂��邩�̏������łɖ߂�����E�ɂ��邩�ǂ����̃`�F�b�N
																if (wall.SetPushWallLength(-1))
																{
																				se.Play(se_select);
																}
																else
																{
																				se.Play(se_cancel);
																}
												}
												//����u���b�N�̔z�u�Z�b�g
												//������ɂ��Ă���͓̂�����������č������Ȃ��悤�ɂ��邽��
												else if (App::GetKeyDown(VK_LEFT))
												{
																se.Play(se_select);
																wall.ChangePushWallLine();
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																//�����������������܂�܂����̂Ŋm�F���s���܂�
																//�����͒Z���V�[���ɂȂ�̂Ńt���O�Ǘ��ł����Ǝv���܂�
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

																//�o���������������o���钷��������
																pMana.SetPushLength(wall.wallData.length);

																//�v���C���[�̈ړ��ꏊ�̐ݒ�
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
																//�����Ō��݂̃J�����̊p�x����݂ăv���C���[�̈ړ����������̂ňړ������𒼂�
																for (int i = 1; i <= 4; i++)
																{
																				if (mainCamera.angles.y - (i * 90.0f) < -45.0f)
																				{
																								moveDirection -= i - 1;
																								if (moveDirection < 0)
																								{
																												//0�ȉ��̎�0�`4�ɖ߂��K�v�����邽��3�ɖ߂�
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
																				//���̃^�[���ɕύX
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
																//�������I�����
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
								
								//��]��315�𒴂�����-45.0�ɖ߂��A�t��������
								//���r���[�Ȑ��l�̗��R�́A�v���C���[�Ȃǂ̈ړ������𒲐�����Ƃ���-45.0����Ă��鐔�l�̕����s������������
								if (mainCamera.angles.y < -45.0)
								{
												mainCamera.angles.y = 315.0f;
								}
								else if (mainCamera.angles.y > 315.0f)
								{
												mainCamera.angles.y = -45.0f;
								}
								//�J�����̃A���O���𐧌�
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

//���̃f�[�^�̓J�����̈ړ��Ȃ̂ł��Ƃ�CameraMove�Ɉڂ��ׂ�
void GameScene::CameraPositionMove()
{
				//�J�����̈ړ������������ǂ����̔���p
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
				//�J�����̈ړ����m�F������ړ��������ȏ�̏ꏊ�ɍs���Ă��Ȃ����𔻒�
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

//�ړ��L�[���������Ƃ��ɕ����ɉ����ăf�[�^��Ԃ�
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
				pMana.Release((float)wall.MaxLength);
				wall.Release();
}