#pragma once

class PlayerManager
{
public:
				PlayerManager();
				~PlayerManager() {};

				//�v���C���[�̍s���@�����ɂǂ���̃^�[������bool�œ����
				void Behavior();
				//�v���C���[�����̃^�[���ړ��ł���ꏊ������
				void MovementRange(Wall* wall, Float3 movePos = Float3(0.0f,0.0f,0.0f),
								int Direction = -1, int moveCount = 1);

				void MoveChack(Wall* wall, int Direction);
				
				void NewTurn() { turn = !turn;}

				void Draw()
				{

				}

				//���v���C���ɏ����n�_�ɖ߂����߂Ɏg��
				void Release(Float3 positionA = Float3(3.0, 0.0f, 4.0f),
								Float3 positionB = Float3(4.0, 0.0f, 3.0f));

private:
				struct Player
				{
								Player();
								//�`��p�̍��W
								Float3 position;
								//�v��̂��s��//�v���C���[���{�b�N�X���~��ŏ������ꍇ�g�p����
								Float3 angles;
								//���̈ړ�������W��`�悷��̂Ɏg�p�i���ʂɂ��g�p�j
								Float3 movePosition;
				};
				Player player[2];
				//�v���C���[����^�[���ɓ������Ƃ̂ł���}�X��
				const int MaxMove = 2;
				//���݂ǂ���̃^�[�����̔���
				bool turn;

				Cube playerCube;

				//MovementRange�Ŋm�F��������ɍ��킹�Ċm�F����ʒu�����炷���߂̔z��
				//����Direction�͉E�ł�//�܂���������͔����v���ɕω����܂�
				//�܂����Ƃ���y��z�Ƃ��Ĉ������ƂɂȂ�̂Œ��ӂ��K�v
				Float2 SearchDirection[4] =
				{
								Float2(1.0f, 0.0f), //�E
								Float2(0.0f, 1.0f), //��
								Float2(-1.0f, 0.0f),//��
								Float2(0.0f,-1.0f) //��
				};
};