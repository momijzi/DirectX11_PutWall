#include"App.hpp"

UIData::UIData()
{
				App::Initialize();
				//���݂̃Q�[���̏�Ԃ������e�N�X�`���@�f�o�b�N�p�ɍ쐬
				Texture gameStateTex(L"texture/testTex.png");

				stateUi.SetTexture(&gameStateTex, Float2(2.0f, 2.0f));
				stateUi.Create(Float2(0.0f, 0.0f));
				stateUi.scale = Float3(App::GetWindowSize().x / 4, App::GetWindowSize().y / 4, 0);
}

void UIData::Draw()
{
				uiCamera.Update();
				stateUi.DrawSprite();
}