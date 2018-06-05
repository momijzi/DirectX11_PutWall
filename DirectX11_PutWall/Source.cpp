#include "App.hpp"

int MAIN()
{
				bool flag = true;

				//Texture texture(L"texture/crab.jpg");
				//texture.Attach(0);

				Texture wallTexture(L"texture/WallTex.png");
				wallTexture.SetDivide(Float2(11.0f, 1.0f));

				//Wall wall(&wallTexture);

				//�v���C���[�̈ړ��X�s�[�h����
				float playerDistance = 1.0f;
				//�ړ��̂��߂�
				Move move;

				Camera camera;
				camera.position = Float3(0.0f, 3.0f, -5.0f);
				camera.angles.x = 10.0f;

				Mesh mesh;
				mesh.CreateData(&wallTexture, Mesh::CreateMode::CUBEOUT);

				int Count = 0;//���ō쐬

				//Model model(L"texture/crab.fbx");

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

								//�v���C���[�̈ړ�
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

								mesh.angles.z -= 0.1f;
								if (mesh.angles.z < -90)
								{
												mesh.angles.z = 0.0f;
								}
								mesh.position.y = 0.25 * sin(2 * mesh.angles.z);
								mesh.Draw(mesh.position,mesh.angles);

								//wall.Draw();
								//model.Draw();
				}

				return 0;
}
