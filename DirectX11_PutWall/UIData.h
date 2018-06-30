#pragma once

class UIData
{
public:
				UIData();
				~UIData(){};
				
				void CreateCutinUi(Float2 numUv = Float2(0.0f, 0.0f));
				
				//UI�̕`��
				//���݂̏�Ԃ̕`��A�f�o�b�N�Ŏg�p
				void DrawTitleUi();
				//�����͎c��̉����o�����
				void DrawNumberUi(float num, Float3 pos);
				//�J�b�g�C���ŕ`�悷����̂܂��J�b�g�C���̎��Ԏ擾
				bool Cutin(int num);
				float GetCountTime() { return countTime; }
				//���E�Ƀv���C���[�̉����o����u���b�N��
				//�^�񒆂ɒ��ނ܂ł̃��~�b�g
				void DrawGaugeBackUi(Float2 numUv, int sign, int num, bool turn);
				//�Q�[���I�[�o�[���̕`��
				void DrawOver(bool turn);
				//��̕`����ꊇ�ōs���܂Ƃ߂�����
				void Draw(int gameState, Float2 playerPushLength,int drowLimitCount, bool turn);
private:
				//2D�f�[�^��`�悷��J����
				Camera uiCamera;
				
				Texture gameTitleTex;
				Plane gameTitleUi;

				Texture numberTex;
				Plane numberUi;

				Texture cutinTex;
				Plane cutinUi;

				Texture gaugeBackTex;
				Plane gaugeBackUi;

				Texture overTex;
				Plane overUi;
				float countTime;
};

