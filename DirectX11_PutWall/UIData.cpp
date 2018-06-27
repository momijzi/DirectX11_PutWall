#include"App.hpp"

UIData::UIData()
{
				App::Initialize();

				gameTitleTex.Load(L"texture/title.png");
				gameTitleUi.Create(&gameTitleTex);
				gameTitleUi.scale = Float3(App::GetWindowSize().x, App::GetWindowSize().y, 0);

				numberTex.Load(L"texture/number.png");
				numberTex.SetDivide(Float2(10.0f, 2.0f));
				CreateNumberUi();
				numberUi.scale = Float3(App::GetWindowSize().x / 9, App::GetWindowSize().y / 6, 0);

				//ここのスケールはtextureから出したいな（出せるし……）
				//でもスクリーンの大きさ変えたら・・うぅ・・
				cutinTex.Load(L"texture/cutinTex.png");
				cutinTex.SetDivide(Float2(1.0f, 3.0f));
				cutinUi.Create(&cutinTex);
				cutinUi.scale = Float3(256, 64, 0);
				cutinUi.position.y = 0;
				countTime = 0.0f;

				uiCamera.position = Float3(0.0f, 0.0f, -1.0f);
}



void UIData::CreateNumberUi(Float2 numUv)
{
				numberTex.SetNumUv(numUv, 0);
				numberUi.Create(&numberTex);
}

void UIData::CreateCutinUi(Float2 numUv)
{
				cutinTex.SetNumUv(numUv, 0);
				cutinUi.Create(&cutinTex);
}

void UIData::DrawTitleUi()
{
				gameTitleUi.DrawSprite();
}
void UIData::DrawNumberUi(Float2 playerPushLength,int drowLimitCount)
{
				//現在のプレイヤーの残り押し出せる量
				//プレイヤーAのデータ左上に
				CreateNumberUi(Float2((float)playerPushLength.x, 0.0f));
				numberUi.position.x = -App::GetWindowSize().x / 2 + numberUi.scale.x / 2;
				numberUi.position.y = App::GetWindowSize().y / 2 - numberUi.scale.y / 2;
				numberUi.DrawSprite();

				CreateNumberUi(Float2((float)playerPushLength.y,1.0f));
				numberUi.position.x = App::GetWindowSize().x / 2 - numberUi.scale.x / 2;
				numberUi.position.y = App::GetWindowSize().y / 2 - numberUi.scale.y / 2;
				numberUi.DrawSprite();

				CreateNumberUi(Float2((float)drowLimitCount, 1.0f));
				numberUi.position.x = 0.0f;
				numberUi.position.y = App::GetWindowSize().y / 2 - numberUi.scale.y / 2;
				numberUi.DrawSprite();
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
void UIData::Draw(int gameState,Float2 playerPushLength,int drowLimitCount)
{
				uiCamera.Update(false);
				switch (gameState)
				{
								case 0://TITLE
												DrawTitleUi();
												break;
								case 1://PLAY
												DrawNumberUi(playerPushLength, drowLimitCount);
												if (countTime != 0)
												{
																cutinUi.Draw();
												}
												break;
								case 2://OVER
												DrawNumberUi(playerPushLength, drowLimitCount);
												break;
				}
}