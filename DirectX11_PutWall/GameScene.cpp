#include"App.hpp"
GameScene::GameScene()
{
				App::Initialize();

				//�}�E�X���g������������ꍇ�͏��������ŏ��ɂ��Ă������Ƃ𐄏�����
				App::SetMousePosition(0.0f, 0.0f);

				mainCamera.position = Float3(0.0f, 5.0f, -10.0f);
}

void GameScene::SceneManager()
{
				mainCamera.Update();
				wall.Draw();
				switch (gameState)
				{
								case GameState::PLAY:
												//MainTurn();
												if (App::GetKeyDown(VK_RETURN))//�ق�Ƃ͂����ɃG���h�t���O��//�Ă�������MainTurn�ɏ������ׂ�
												{
																uiData.CreateStateUi(Float2(1.0f, 1.0f));
																gameState = OVER;
												}
												if (App::GetKeyDown('A'))
												{
																wall.ResetPlayerMoveFlag();
																playerManager.NextTurn();
																playerManager.MovementRange(&wall);
												}
												wall.Draw();
												playerManager.Draw();
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
												//���̃v���C���[���ړ��ł���ꏊ���i�[
												wall.ResetPlayerMoveFlag();
												playerManager.NextTurn();
												playerManager.MovementRange(&wall);
												scene = PLAYER_MOVE;
												//�����ɃA�j���[�V�����i��U�̃^�[���݂����ȁj����ꂽ����
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
																				scene = PUSH_WALL_SELECT;
																}
																//�܂��ړ����������ĂȂ��ł��B�݂����Ȃ̗~�����ȁ`
												}
								}
												break;
								case GameScene::PUSH_WALL_SELECT:
												//�����o���ǂ�I������@�\���őI���ɂȂ�Ǝv����
												//�I������Ƃ���y�������ړ��̒��S����ǂ��m�F����
												if (true)
												{
																//�I�������ꏊ�ŉ����o�����Ƃ��\�Ȃ��Ƃ��킩��܂���
																scene = SET_PUSH_WALL_LENGTH;
												}
												else
												{
																//�I�������ꏊ�����łɉ����o����Ă����ӏ��ł�����
																//�Ԃ��ԁ[�I�ȁH
												}

												break;
								case GameScene::SET_PUSH_WALL_LENGTH:
												if (true)
												{
																//�����o���������܂����߂Ă��Ȃ�
																if (true)//�Ȃɂ��ŕǂ��牟���o���u���b�N�̒��������߂�
																{

																}

																if (App::GetKeyDown(VK_RETURN))
																{
																				//�����������������܂�܂����̂Ŋm�F���s���܂�
																				//�����͒Z���V�[���ɂȂ�̂Ńt���O�Ǘ��ł����Ǝv���܂�
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
												Wall::WallData wallData(testSurface, testWidth, testHeight, testLength, testCount);
												wall.SetInitialPosition(wallData);
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
				//�v���C���[�̈ړ�
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
				if (App::GetKeyDown(VK_RIGHT))
				{
								return 0;
				}
				else if (App::GetKeyDown(VK_UP))
				{
								return 1;
				}
				else if (App::GetKeyDown(VK_LEFT))
				{
								return 2;
				}
				else if (App::GetKeyDown(VK_DOWN))
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