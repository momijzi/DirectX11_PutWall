#include "App.hpp"

int MAIN()
{
				//����R���t�B�O�g�����s��
				enum GameState{TITLE,CONFIG,PLAY,OVER};
				GameState gameState = TITLE;

				Texture stateTex(L"texture/testTex.png");
				stateTex.SetDivide(Float2(2.0f, 2.0f));
				Texture wallTex(L"texture/StayBox.png");
				//���݂̃Q�[���̏�Ԃ�\���e�N�X�`���̕`��iUI�j
				Plane titleUI;
				titleUI.Create(&stateTex,Float2(0.8f,0.6f));
				//�ړ��̂��߂�
				Move move;

				Camera camera;
				camera.position = Float3(0.0f, 0.0f,-1.0f);

				Wall wall(&wallTex);

				App::SetMousePosition(0.0f, 0.0f);

				while (App::Refresh())
				{
								if (App::GetKey(VK_LBUTTON))
								{
												if (App::GetKeyDown(VK_LBUTTON))
												{
																App::SetMousePosition(0.0f, 0.0f);
												}
												camera.angles += Float3(
																-App::GetMousePosition().y,
																App::GetMousePosition().x,
																0.0f)*0.1f;

												move.angles += Float3(
																-App::GetMousePosition().y,
																App::GetMousePosition().x,
																0.0f)*0.1f;

												App::SetMousePosition(0.0f, 0.0f);
								}
								camera.Update();
								switch (gameState)
								{
												case PLAY:
																if (App::GetKeyDown(VK_RETURN))//�ق�Ƃ͂����ɃG���h�t���O��
																{
																				stateTex.SetNumUvFront(Float2(1.0f, 1.0f));
																				titleUI.Create(&stateTex);
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
																								stateTex.SetNumUvFront(Float2(1.0f, 0.0f));
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
																								stateTex.SetNumUvFront(Float2(0.0f, 1.0f));
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
																titleUI.Draw();
																break;
								}
				}
				return 0;
}
