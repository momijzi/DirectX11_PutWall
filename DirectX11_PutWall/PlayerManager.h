#pragma once

class PlayerManager
{
public:
				PlayerManager();
				~PlayerManager() {};
				
				//�v���C���[�̍s���@�����ɂǂ���̃^�[������bool�œ����
				void Behavior();
				void MovementRange(Wall* wall);
				//���v���C���ɏ����n�_�ɖ߂����߂Ɏg��
				void Release();

private:
				struct Player
				{
								Player();
								Float3 position;//�`��p�̍��W
								Float3 angles;//�v��̂��s��//�v���C���[���{�b�N�X���~��ŏ������ꍇ�g�p����
								Float3 movePosition;//���̈ړ�������W��`�悷��̂Ɏg�p�i���ʂɂ��g�p�j
				};
				Player player[2];
				const int Maxmove = 2;//�v���C���[����^�[���ɓ������Ƃ̂ł���}�X��
				bool turn;//���݂ǂ���̃^�[�����̔���

};