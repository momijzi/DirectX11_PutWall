#include"App.hpp"

UIData::UIData()
{
				App::Initialize();

				gameStateTex.Load(L"texture/testTex.png");
				gameStateTex.SetDivide(Float2(2.0f, 2.0f));
				stateUi.Create(&gameStateTex);
				stateUi.scale = Float3(App::GetWindowSize().x / 4, App::GetWindowSize().y / 4, 0);

				uiCamera.position = Float3(0.0f, 0.0f, -1.0f);
}

void UIData::CreateStateUi(Float2 numUv)
{
				gameStateTex.SetNumUv(numUv, 0);
				stateUi.Create(&gameStateTex);
}

void UIData::Draw()
{
				uiCamera.Update(false);
				stateUi.position.x = App::GetWindowSize().x / 2 - stateUi.scale.x / 2;
				stateUi.position.y = App::GetWindowSize().y / 2 - stateUi.scale.y / 2;//+は上に行く　何座標系だったか……調べる！
				stateUi.DrawSprite();
}