#include"App.hpp"
GameScene::GameScene()
{
				App::Initialize();

				//�}�E�X���g������������ꍇ�͏��������ŏ��ɂ��Ă������Ƃ𐄏�����
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
												//if (App::GetKeyDown(VK_RETURN))//�ق�Ƃ͂����ɃG���h�t���O��//�Ă�������MainTurn�ɏ������ׂ�
												//{
												//				uiData.CreateStateUi(Float2(1.0f, 1.0f));
												//				gameState = OVER;
												//}
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
				uiData.Draw();
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
																				wall.SelectLookWall(testHeight, mainCamera.angles.y, testSurface, testWidth, testFlag);
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
																wall.SelectToWall(testSurface, testHeight, testWidth, testDirection, testWallFlag);
												}
												
												if (App::GetKeyDown(VK_RETURN))
												{
																if (testWallFlag)
																{
																				//�I�������ꏊ�ŉ����o�����Ƃ��\�Ȃ��Ƃ��킩��܂���
																				scene = SET_PUSH_WALL_LENGTH;
																}
																else
																{
																				//�I�������ꏊ�����łɉ����o����Ă����ӏ��ł�����
																				//�Ԃ��ԁ[�I�ȁH
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
																//�����o���������܂����߂Ă��Ȃ�
																if (true)//�Ȃɂ��ŕǂ��牟���o���u���b�N�̒��������߂�
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
																				//�����������������܂�܂����̂Ŋm�F���s���܂�
																				//�����͒Z���V�[���ɂȂ�̂Ńt���O�Ǘ��ł����Ǝv���܂�

																				//���ؗp�A��ŏ�������_�v�ۓc
																				wallData.SetWallData(testSurface, testWidth, testHeight, testLength, testCount);
																				wall.SetInitialPosition(wallData);
																				scene = TURN_END;
																}
												}
												else
												{
																//�����o�����������肷��
																//���S�Ȍ���
																//�����͂����炭ENTER���ƌ���ĉ����Ă��܂��\�������邽�ߒ���
																if (App::GetKeyDown(VK_RETURN))
																{
																				scene = TURN_END;
																}
												}
												break;
								case GameScene::CAMERA_MOVE_CHACK:
												//�J�����𓮂����Ď�����m�F����
												if (KeyMoveData() != -1)
												{
																//�ړ��������Ƃ��킩��
																//����͔��̒������ړ����Ȃ��\��Ȃ̂�
																//�����͂��������
												}
												break;
								case GameScene::TURN_END:
												//�^�[���G���h�̐錾������ꏊ�@������false��Ԃ���MoveChack�Ɉړ�
								{
												//�����ɑI�����Ă����������΂���
												wall.MoveWall(wallData);
								}

								if (false)//�����o���̏������I��������ǂ���
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
				//�J�����̈ړ�
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