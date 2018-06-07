#include"App.hpp"

/*
これから追加する事
Rayの追加（視点方向の選択）
プレイヤーのモデルのサイズ調整
シェーダ―にalphaを追加したい
UIの追加だが大きさの設定をどうするか
キーコンフィグを追加してみたい
ゲームの流れを作成（早急）
幸福は義務なのです（重要）
*/

int MAIN()
{
				enum GameState { TITLE, CONFIG, PLAY, OVER };
				GameState game = TITLE;

				Texture wallTexture(L"texture/WallTex.png");
				wallTexture.SetDivide(Float2(11.0f, 1.0f));

				Texture stateUI(L"testTexture.png");
				stateUI.SetDivide(Float2(2.0f, 2.0f));

				////プレイヤーの移動スピードかな
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

