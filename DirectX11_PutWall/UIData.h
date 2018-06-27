#pragma once

class UIData
{
public:
				UIData();
				~UIData() {};
				
				void CreateNumberUi(Float2 numUv = Float2(0.0f, 0.0f));
				void CreateCutinUi(Float2 numUv = Float2(0.0f, 0.0f));
				//UI�̕`��
				//���݂̏�Ԃ̕`��A�f�o�b�N�Ŏg�p
				void DrawTitleUi();
				//�����͎c��̉����o�����
				void DrawNumberUi(Float2 playerPushLength, int drowLimitCount);
				//�J�b�g�C���ŕ`�悷�����
				bool Cutin(int num);
				//��̕`����ꊇ�ōs���܂Ƃ߂�����
				void Draw(int gameState, Float2 playerPushLength,int drowLimitCount);
private:
				//2D�f�[�^��`�悷��J����
				Camera uiCamera;
				
				Texture gameTitleTex;
				Plane gameTitleUi;
				Texture numberTex;
				Plane numberUi;
				Texture cutinTex;
				Plane cutinUi;

				float countTime;
};

