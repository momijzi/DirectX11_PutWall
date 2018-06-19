#include"App.hpp"

//Camera ��LookPos�œ��ꂵ�Ă݂Ă����������H

//���݂̃o�O
//�v���C���[�̈ړ������Ă���I���Ɉړ�����Ƃ��Ƀo�O����������
//�Ȃ����s��
//�����z��O�ɍs�����Ƃ��������Ƃ͂킩����


GameScene::GameScene()
{
				App::Initialize();

				//�}�E�X���g������������ꍇ�͏��������ŏ��ɂ��Ă������Ƃ𐄏�����
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
												//if (App::GetKeyDown(VK_RETURN))//�ق�Ƃ͂����ɃG���h�t���O��//�Ă�������MainTurn�ɏ������ׂ�
												//{
												//				uiData.CreateStateUi(Float2(1.0f, 1.0f));
												//				gameState = OVER;
												//}
												mainCamera.Update(true);
												playerManager.Draw(wall.length, wall.blockSize);
												wall.Draw(playerManager.GetPlayerDrawFlag());
												break;
								default:
												//��ɕ`�悵���ύX���Ȃ��̂ňꊇ�ŕύX����
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
												//�����ɃA�j���[�V�����i��U�̃^�[���݂����ȁj����ꂽ����

												//���̃v���C���[���ړ��ł���ꏊ���i�[
												//�����ɃA�j���[�V�������I���������scene���ς��悤�ɂ���flag������
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
												//�\���L�[�ňړ��Ƃ���ړ������̓��͔͂z�񏇎�Ƃ���
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
																//�܂��ړ����������ĂȂ��ł��B�݂����Ȃ̗~�����ȁ`
												}
								}
								break;
								case GameScene::PUSH_WALL_SELECT:
												//�����o���ǂ�I������@�\���őI���ɂȂ�Ǝv����
												//�I������Ƃ���y�������ړ��̒��S����ǂ��m�F����
												testDirection = KeyMoveData() + 1;
												if (testDirection != 0)
												{
																wall.SelectToWall(testDirection);
												}
												if (App::GetKeyDown(VK_RETURN))
												{
																if (wall.wallData.drawTexFlag)
																{
																				//�I�������ꏊ�ŉ����o�����Ƃ��\�Ȃ��Ƃ��킩��܂���
																				wall.SetInitialPosition();
																				wall.wallData.checkLengthFlag = true;
																				scene = SET_PUSH_WALL_LENGTH;
																}
																else
																{
																				//�I�������ꏊ�����łɉ����o����Ă����ӏ��ł�����
																				//�Ԃ��ԁ[�I�ȁH
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
												//�����o���������܂����߂Ă��Ȃ�

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
																//�����������������܂�܂����̂Ŋm�F���s���܂�
																//�����͒Z���V�[���ɂȂ�̂Ńt���O�Ǘ��ł����Ǝv���܂�

																wall.wallData.checkLengthFlag = false;
																wall.wallData.moveFlag = true;
																
																playerManager.DeliverLength(wall.wallData.length);
																scene = TURN_END;
												}

												break;
								case GameScene::TURN_END:
												//�^�[���G���h�̐錾������ꏊ�@������false��Ԃ���MoveChack�Ɉړ�
								
												//�����ɑI�����Ă����������΂���
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