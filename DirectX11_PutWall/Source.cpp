#include "App.hpp"

int MAIN()
{
				//今回コンフィグ使うか不明
				enum GameState{TITLE,CONFIG,PLAY,OVER};
				GameState gameState = TITLE;

				Texture stateTex(L"texture/testTex.png");
				stateTex.SetDivide(Float2(2.0f, 2.0f));
				Texture wallTex(L"texture/StayBox.png");

				//移動のための
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
																//主に描画しか変更がないので一括で変更する


																break;
								}


								camera.Update();
								wall.Draw();
				}
				return 0;
}
