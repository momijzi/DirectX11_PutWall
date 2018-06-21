#pragma once

class UIData
{
public:
				UIData();
				~UIData() {};
				
				void CreateStateUi(Float2 numUv = Float2(0.0f, 0.0f));
				void CreateNumberUi(Float2 numUv = Float2(0.0f, 0.0f));

				//UI�̕`��
				//���݂̏�Ԃ̕`��A�f�o�b�N�Ŏg�p
				void DrawStateUi();
				//�����͎c��̉����o�����
				void DrawNumberUi(Float2 playerPushLength);
				//��̕`����ꊇ�ōs���܂Ƃ߂�����
				void Draw(Float2 playerPushLength);
private:
				//2D�f�[�^��`�悷��J����
				Camera uiCamera;
				
				Texture gameStateTex;
				Plane gameStateUi;
				Texture numberTex;
				Plane numberUi;
};

