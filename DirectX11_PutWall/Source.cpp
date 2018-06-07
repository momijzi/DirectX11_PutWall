#include"App.hpp"

/*
���ꂩ��ǉ����鎖
Ray�̒ǉ��i���_�����̑I���j
�v���C���[�̃��f���̃T�C�Y����
�V�F�[�_�\��alpha��ǉ�������
UI�̒ǉ������傫���̐ݒ���ǂ����邩
�L�[�R���t�B�O��ǉ����Ă݂���
�Q�[���̗�����쐬�i���}�j
�K���͋`���Ȃ̂ł��i�d�v�j
*/

int MAIN()
{
				enum GameState { TITLE, CONFIG, PLAY, OVER };
				GameState game = TITLE;

				Texture wallTexture(L"texture/WallTex.png");
				wallTexture.SetDivide(Float2(11.0f, 1.0f));

				Texture stateUI(L"testTexture.png");
				stateUI.SetDivide(Float2(2.0f, 2.0f));

				////�v���C���[�̈ړ��X�s�[�h����
				//float playerDistance = 1.0f;

				Camera camera;
				camera.position.z = -3.0f;

				Mesh mesh;

				while (App::Refresh())
				{
								switch (game)
								{
												case GameState::TITLE:
																if (App::GetKeyDown(VK_RETURN))
																{
																				game = PLAY;
																}
																else if (App::GetKeyDown(VK_SPACE))
																{
																				game = CONFIG;
																}
																break;
												case GameState::CONFIG:
																if (App::GetKeyDown(VK_RETURN))
																{
																				game = TITLE;
																}
																break;
												case GameState::PLAY:
																if (App::GetKeyDown(VK_RETURN))
																{
																				game = OVER;
																}

																break;
												case GameState::OVER:
																if (App::GetKeyDown(VK_RETURN))
																{
																				game = TITLE;
																}
																break;
												default:
																break;
								}
								camera.Update();
				}
				return 0;
}

