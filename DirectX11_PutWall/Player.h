#pragma once
class Player
{
				Player(Texture* tex);
				
				~Player()
				{

				}

				//�ړ��ł��邩�̊m�F ������ 1 or -1 ��������Ă͂����Ȃ�
				void MoveConfirmation(int depth, int side);
				
				//�v���C���[�̕`��
				void Draw();

private:
				Wall wall;
				Mesh mesh;
				//�`��p�̍��W
				Float3 position;
				Float3 angles;//�v��̂��s��
																		//�ړ����鋗�����i�[����
				Float3 movePosition;//���̈ړ�������W��`�悷��̂Ɏg�p�i���ʂɂ��g�p�j
				const int Maxmove = 2;//�v���C���[����^�[���ɓ������Ƃ̂ł���}�X��

};