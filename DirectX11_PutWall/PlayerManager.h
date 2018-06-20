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

				//���̃v���C���[�̃^�[���ɂ���
				void NextTurn() 
				{ 
								player[turn].movePosition = 0.0f;
								turn = !turn; 
				}
				void SetPlayerDrawFlag(bool flag)
				{
								player[turn].moveFlag = flag;
				}
				bool GetPlayerDrawFlag()
				{
								return player[turn].moveFlag;
				}
				//���݃^�[�������Ă���v���C���[�̉����o�����
				int GetCurrentPlayerPushLength() { return player[turn].length; }

				Float3 GetPlayerPosition(int playerNum) { return player[playerNum].position; }

				//UI��`�悷�邽�߂Ɏg�p
				Float2 GetPlayerPushLength()
				{
								return Float2((float)player[0].length, (float)player[1].length);
				}

				//�ړ����������Ă�����
				void MoveableChack(Wall* wall, int Direction);
				
				//��}�X�ȏ�̈ړ������Ă��邩
				bool MoveFlagChack();
				//�ړ�������ɍēx�ړ��ꏊ��ύX�������Ƃ����̊֐����Ă�Ŗ߂�
				void ReturnMovePos();

				//�����o�������܂����̂ŉ����o�������𑊎�ɏ��n���܂�
				void DeliverLength(int length);

				void Draw(int boxLength, int blockSize);

				//���v���C���ɏ����n�_�ɖ߂����߂Ɏg��
				void Release(Float3 positionA = Float3(3.0, 0.0f, 4.0f),
								Float3 positionB = Float3(4.0, 0.0f, 3.0f));

private:
				struct Player
				{
								Player();
								//�`��p�̍��W
								Float3 position;
								//���̈ړ�������W��`�悷��̂Ɏg�p�i���ʂɂ��g�p�j
								Float3 movePosition;
								//�ړ������߂Ă���Œ����ǂ����̃t���O
								//����ŉ��̃v���C���[��`�悷�邩�����߂�
								//������Alpha�𔼓����ɂ��������ł��Ȃ��̂łȂ�Ƃ��E�E
								bool moveFlag;
								//�����o����u���b�N�̗�
								int length;
				};
				Player player[2];
				//�v���C���[����^�[���ɓ������Ƃ̂ł���}�X��
				const int MaxMove = 2;
				//���݂ǂ���̃^�[�����̔���
				bool turn;

				Texture playerTex;
				Cube playerCube[2];

				//MovementRange�Ŋm�F��������ɍ��킹�Ċm�F����ʒu�����炷���߂̔z��
				//����Direction�͉E�ɂ������������Awall�̑I���ƈႤ�����ɂȂ��Ă��邽�߂�ނȂ��ς���
				//�܂���������͔����v���ɕω����܂�
				//�܂����Ƃ���y��z�Ƃ��Ĉ������ƂɂȂ�̂Œ��ӂ��K�v
				Float2 SearchDirection[4] =
				{
								Float2(0.0f, 1.0f), //��
								Float2(-1.0f, 0.0f),//��
								Float2(0.0f,-1.0f), //��
								Float2(1.0f, 0.0f)  //�E
				};
};