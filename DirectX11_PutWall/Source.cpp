#include "App.hpp"

int MAIN()
{
				//����R���t�B�O�g�����s��
				enum GameState{TITLE,CONFIG,PLAY,OVER};
				GameState gameState = TITLE;

				Texture stateTex(L"texture/testTex.png");
				stateTex.SetDivide(Float2(2.0f, 2.0f));
				Texture wallTex(L"texture/StayBox.png");

				//�ړ��̂��߂�
				Move move;

				Camera camera;
				camera.position = Float3(12.0f, 0.0f,-12.0f);
				camera.angles.y = -45.0f;

				Wall wall(&wallTex);

				App::SetMousePosition(0.0f, 0.0f);

				while (App::Refresh())
				{
								/*if (App::GetKey(VK_RETURN))
								{
												return 0;
								}
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
								}*/
								switch (gameState)
								{
												case PLAY:
																break;
												default:
																//��ɕ`�悵���ύX���Ȃ��̂ňꊇ�ŕύX����


																break;
								}


								camera.Update();
								wall.Draw();
				}
				return 0;
}
