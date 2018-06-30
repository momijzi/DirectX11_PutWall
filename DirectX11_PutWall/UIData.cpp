#include"App.hpp"

UIData::UIData()
{
				App::Initialize();

				//タイトル画像のセット
				gameTitleTex.Load(L"texture/title.png");
				gameTitleUi.Create(&gameTitleTex);
				gameTitleUi.scale = Float3(App::GetWindowSize().x, App::GetWindowSize().y, 0);

				//数字の画像セット
				numberTex.Load(L"texture/number.png");
				numberTex.SetDivide(Float2(10.0f, 1.0f));
				numberUi.scale = Float3(App::GetWindowSize().x / 9, App::GetWindowSize().y / 6, 0);

				//カットインの画像のセット
				cutinTex.Load(L"texture/turn.png");
				cutinTex.SetDivide(Float2(1.0f, 2.0f));
				cutinUi.Create(&cutinTex);
				cutinUi.scale = Float3(256, 64, 0);
				cutinUi.position.y = 0;
				countTime = 0.0f;
				//ゲージのバックの板のセット
				gaugeBackTex.Load(L"texture/gaugeBack.png");
				gaugeBackTex.SetDivide(Float2(4.0f, 1.0f));
				gaugeBackUi.scale = Float3(App::GetWindowSize().x / 9, App::GetWindowSize().y / 6, 0);

				//ゲームオーバー時にどちらが勝ったかを示す画像のセット
				overTex.Load(L"texture/win.png");
				overTex.SetDivide(Float2(1.0f, 2.0f));
				overUi.scale = Float3(256, 64, 0);

				uiCamera.isDepthTest = false;
				uiCamera.position = Float3(0.0f, 0.0f, -1.0f);
}
void UIData::CreateCutinUi(Float2 numUv)
{
				cutinTex.SetNumUv(numUv, 0);
				cutinUi.Create(&cutinTex);
}
void UIData::DrawTitleUi()
{
				gameTitleUi.Draw();
}

//boolはどちらのプレイヤーのデータを描画するか
void UIData::DrawGaugeBackUi(Float2 numUv, int sign, int num,bool turn)
{
				gaugeBackTex.SetNumUv(numUv, 0.0f);
				gaugeBackUi.Create(&gaugeBackTex);
				gaugeBackUi.position.x = App::GetWindowSize().x / 2 * sign - gaugeBackUi.scale.x / 2 * sign;
				gaugeBackUi.position.y = App::GetWindowSize().y / 2 - gaugeBackUi.scale.y / 2;

				gaugeBackUi.Draw();
				if ((int)turn == (int)numUv.x)
				{
								gaugeBackTex.SetNumUv(Float2(3.0f,0.0f), 0.0f);
								gaugeBackUi.Create(&gaugeBackTex);
								gaugeBackUi.Draw();
				}
				DrawNumberUi((float)num, gaugeBackUi.position);
}

void UIData::DrawOver(bool turn)
{
				overTex.SetNumUv(Float2(0.0f, (float)turn), 0);
				overUi.Create(&overTex);
				overUi.Draw();
}

void UIData::DrawNumberUi(float num, Float3 pos)
{
				//0 ～ 9 までの数字を描画可能
				//現在のプレイヤーの残り押し出せる量
				//プレイヤーAのデータ左上に
				numberTex.SetNumUv(Float2(num, 1.0f), 0);
				numberUi.Create(&numberTex);
				numberUi.position = pos;
				numberUi.Draw();
}

bool UIData::Cutin(int num)
{
				if (countTime == 0)
				{
								CreateCutinUi(Float2(0.0f, (float)num));
				}
				countTime += App::GetDeltaTime();
				switch ((int)countTime)
				{
								case 0:
												cutinUi.position.x = App::GetWindowSize().x / 4 * 3 * countTime - App::GetWindowSize().x / 4 * 3;
												break;
								case 1:
												cutinUi.position.x = 0;
												break;
								case 2:
												cutinUi.position.x = App::GetWindowSize().x / 4 * 3 * (countTime - 2);
												break;
								default:
												countTime = 0;
												return true;
												break;
				}
				return false;
}
void UIData::Draw(int gameState,Float2 playerPushLength,int drowLimitCount, bool turn)
{
				uiCamera.Update(false);
				switch (gameState)
				{
								case 0://TITLE
												DrawTitleUi();
												break;
								case 1://PLAY
												DrawGaugeBackUi(Float2(0.0f, 0.0f),-1, (int)playerPushLength.x, turn);
												DrawGaugeBackUi(Float2(1.0f, 0.0f), 1, (int)playerPushLength.y, turn);
										 	DrawGaugeBackUi(Float2(2.0f, 0.0f), 0, drowLimitCount, turn);
												if (countTime != 0)
												{
																cutinUi.Draw();
												}
												break;
								case 2://OVER
												DrawOver(turn);
												break;
				}
}