#include "App.hpp"

int MAIN()
{
				//����R���t�B�O�g�����s��
				enum GameState{TITLE,CONFIG,PLAY,OVER};
				GameState gameState = TITLE;

				Texture stateTex(L"texture/testTex.png");
				stateTex.SetDivide(Float2(2.0f, 2.0f));
				Texture wallTex(L"texture/StayBox.png");
				wallTex.SetDivide(Float2(2.0f, 1.0f));
				//���݂̃Q�[���̏�Ԃ�\���e�N�X�`���̕`��iUI�j
				Plane titleUI;
				titleUI.Create(&stateTex);
				titleUI.scale = Float3(App::GetWindowSize().x / 4, App::GetWindowSize().y / 4, 0);
				//�ړ��̂��߂�
				Move move;
				//�J��������
				Camera mainCamera;
				mainCamera.position = Float3(0.0f, 5.0f,-10.0f);
				Camera uiCamera(false);
				uiCamera.position = Float3(0.0f, 0.0f, -1.0f);

				//Wall�̉����o�����s�������̎����p�Ɏg�p
				int testSurface = 0;
				int testWidth = 3;
				int testHeight = 4;
				int testLength = 5;
				Float3 testPos[5] = {};
				bool testFlag = false;
				float testCount = 0.0f;

				Wall wall(&wallTex);
				Wall::WallData wallData(testSurface, testWidth, testHeight, testLength, testCount);
				wall.SetInitialPosition(wallData);

				PlayerManager playerManager;

				App::SetMousePosition(0.0f, 0.0f);

				while (App::Refresh())
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

								mainCamera.Update();
								switch (gameState)
								{
												case PLAY:
																if (App::GetKeyDown(VK_RETURN))//�ق�Ƃ͂����ɃG���h�t���O��
																{
																				stateTex.SetNumUvFront(Float2(1.0f, 1.0f));
																				titleUI.Create(&stateTex);
																				gameState = OVER;
																}
																if (App::GetKeyDown('A'))
																{
																			wall.ResetPlayerMoveFlag();
																			playerManager.NewTurn();
																			playerManager.MovementRange(&wall);
																}
																wall.Draw();
																break;
												default:
																
																//��ɕ`�悵���ύX���Ȃ��̂ňꊇ�ŕύX����
																if (App::GetKeyDown(VK_RETURN))
																{
																				if (gameState == TITLE)
																				{
																								stateTex.SetNumUvFront(Float2(0.0f, 1.0f));
																								titleUI.Create(&stateTex);
																								gameState = PLAY;
																				}
																				else if (gameState == OVER)
																				{
																								stateTex.SetNumUvFront(Float2(0.0f, 0.0f));
																								titleUI.Create(&stateTex);
																								gameState = TITLE;
																				}
																}
																else if (App::GetKeyDown(VK_SPACE))
																{
																				if (gameState != CONFIG)
																				{
																								stateTex.SetNumUvFront(Float2(1.0f, 0.0f));
																								titleUI.Create(&stateTex);
																								gameState = CONFIG;
																				}
																				else
																				{
																								stateTex.SetNumUvFront(Float2(0.0f, 0.0f));
																								titleUI.Create(&stateTex);
																								gameState = TITLE;
																				}
																}
																break;
								}
								uiCamera.Update();
								titleUI.position.x = App::GetWindowSize().x / 2 - titleUI.scale.x / 2;
								titleUI.position.y = App::GetWindowSize().y / 2 - titleUI.scale.y / 2;//+�͏�ɍs���@�����W�n���������c�c���ׂ�I
								titleUI.DrawSprite();
				}
				
				return 0;
}
