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
				switch (gameState)
				{
								case PLAY:
												MainTurn();
												if (App::GetKeyDown(VK_RETURN))//�ق�Ƃ͂����ɃG���h�t���O��//�Ă�������MainTurn�ɏ������ׂ�
												{
																uiData.stateUi.Create(Float2(1.0f, 1.0f));
																gameState = OVER;
												}
												wall.Draw();
												break;
								default:

												//��ɕ`�悵���ύX���Ȃ��̂ňꊇ�ŕύX����
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
												//���̃v���C���[���ړ��ł���ꏊ���i�[
												wall.ResetPlayerMoveFlag();
												playerManager.NewTurn();
												playerManager.MovementRange(&wall);

												//�����ɃA�j���[�V�����i��U�̃^�[���݂����ȁj����ꂽ����
												break;
								case GameScene::PLAYER_MOVE:
												//�\���L�[�ňړ��Ƃ���ړ������̓��͔͂z�񏇎�Ƃ���

												break;
								case GameScene::PUSH_WALL_SELECT:
												//�����o���ǂ�I������@�\���őI���ɂȂ�Ǝv����

												break;
								case GameScene::SET_PUSH_WALL_LENGTH:

												break;
								case GameScene::MOVE_CHACK:
												//�J�����𓮂����Ď�����m�F����

												break;
								case GameScene::TURN_END:
												//�^�[���G���h�̐錾������ꏊ�@������false��Ԃ���MoveChack�Ɉړ�
								{
												//�����ɑI�����Ă����������΂���
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

void GameScene::Release()
{
				playerManager.Release();
				wall.Release();
}