#include"App.hpp"

UIData::UIData()
{
				App::Initialize();

				gameStateTex.Load(L"texture/testTex.png");
				gameStateTex.SetDivide(Float2(2.0f, 2.0f));
				CreateStateUi();
				gameStateUi.scale = Float3(App::GetWindowSize().x / 6, App::GetWindowSize().y / 6, 0);

				numberTex.Load(L"texture/number.png");
				numberTex.SetDivide(Float2(10.0f, 2.0f));
				CreateNumberUi();
				numberUi.scale = Float3(App::GetWindowSize().x / 9, App::GetWindowSize().y / 6, 0);

				uiCamera.position = Float3(0.0f, 0.0f, -1.0f);
}

void UIData::CreateStateUi(Float2 numUv)
{
				gameStateTex.SetNumUv(numUv, 0);
				gameStateUi.Create(&gameStateTex);
}

void UIData::CreateNumberUi(Float2 numUv)
{
				numberTex.SetNumUv(numUv, 0);
				numberUi.Create(&numberTex);
}

void UIData::DrawStateUi()
{
				//この中で画面のどの位置に描画するか決めること
				//ウィンドウサイズの変更にも対応できるように
				gameStateUi.position.x = -App::GetWindowSize().x / 2 + gameStateUi.scale.x / 2;
				gameStateUi.position.y = -App::GetWindowSize().y / 2 + gameStateUi.scale.y / 2;//+は上に行く　何座標系だったか……調べる！
				gameStateUi.DrawSprite();
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

void UIData::Draw(Float2 playerPushLength,int drowLimitCount)
{
				uiCamera.Update(false);
				DrawNumberUi(playerPushLength, drowLimitCount);
			 DrawStateUi();
}