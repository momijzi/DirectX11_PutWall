#include "App.hpp"

int MAIN()
{
				bool flag = true;

				//Texture texture(L"texture/crab.jpg");
				//texture.Attach(0);

				Texture texture(L"texture/StayBox.png");

				//プレイヤーの移動スピードかな
				float playerDistance = 1.0f;
				//移動のための
				Move move;

				Camera camera;
				camera.position = Float3(0.0f, 3.0f, -5.0f);
				camera.angles.x = 10.0f;

				Mesh mesh;
				mesh.CreateData(&texture, Mesh::CreateMode::CUBEOUT);
				mesh.scale = 5.0f;

				Model model(L"texture/crab.fbx");

				App::SetMousePosition(0.0f, 0.0f);

				while (App::Refresh())
				{
								if (App::GetKey(VK_RETURN))
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
								}

								//プレイヤーの移動
								if (App::GetKey(VK_UP))
								{
												camera.position = move.MovePos(camera.position, true, false, true, true);
								}
								else if (App::GetKey(VK_DOWN))
								{
												camera.position = move.MovePos(camera.position, false, false, true, true);
								}
								if (App::GetKey(VK_RIGHT))
								{
												camera.position = move.MovePos(camera.position, true, true, true, false);
								}
								else if (App::GetKey(VK_LEFT))
								{
												camera.position = move.MovePos(camera.position, false, true, true, false);
								}

								camera.Update();

								mesh.Draw();

								//model.Draw();
				}

				return 0;
}
