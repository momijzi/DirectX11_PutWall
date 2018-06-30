#pragma once

class PlayerManager
{
public:
				PlayerManager();
				~PlayerManager() {};

				//�v���C���[�����݈ړ��ꏊ��I�����Ă����Ԃ����Z�b�g����
				void SetDrawFlag(bool flag) { player[turn].moveFlag = flag; }
				//��̎擾�o�[�W����
				bool GetDrawFlag() { return player[turn].moveFlag; }
				//���݃^�[�������Ă���v���C���[�̉����o�����
				int GetCurrentTurnPushLength() { return player[turn].length; }
				//�ǂ������o�����̂ŉ����o��������������
				void SetPushLength(int length) { player[turn].length -= length; }
				//�����Ɍ��݂̃X�e�[�W�̑傫�������Ă���
				//����ŃX�e�[�W�̑傫�����ő�ێ��ʂƂ��ĕۊǂ���
				void PlusPushLength(int wallLength);
			
				//turn�����Ă���v���C���[�̍��W�����ĂȂ����W���w�肵�ēn��
				Float3 GetPosition(bool flag);
				
				//���݃^�[�������Ă���v���C���[�̎擾
				bool GetTurn() { return turn; }

				//UI��`�悷�邽�߂Ɏg�p
				Float2 GetPushLength() { return Float2((float)player[0].length, (float)player[1].length); }

				//�v���C���[�����̃^�[���ړ��ł���ꏊ������
				void MovementRange(Wall* wall, Float3 movePos = Float3(0.0f, 0.0f, 0.0f),
								int Direction = -1, int moveCount = 1);

				//�ړ����������Ă�����
				void MoveableChack(Wall* wall, int Direction);
				
				//��}�X�ȏ�̈ړ������Ă��邩
				bool MoveFlagChack();

				//�X�e�[�W�����~������
				bool DownPlayer();

				//���̃v���C���[�̃^�[���ɂ���
				void NextTurn()
				{
								player[turn].movePosition = 0.0f;
								turn = !turn;
				}

				//�v���C���[�̕`��
				void Draw(int boxLength, int blockSize,float downPos);

				//���v���C���ɏ����n�_�ɖ߂����߂Ɏg��
				void Release(float mapLength);

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
				const int plusPoint = 3;
				//���݂ǂ���̃^�[�����̔���
				bool turn;
				//�v���C���[����
				Texture playerTex;
				Cube playerCube[2];

				//MovementRange�Ŋm�F��������ɍ��킹�Ċm�F����ʒu�����炷���߂̔z��
				//����Direction�͉E�ɂ������������Awall�̑I���ƈႤ�����ɂȂ��Ă��邽�߂�ނȂ��ς���
				//�܂���������͔����v���ɕω����܂�
				//�܂����Ƃ���y�𐔒l�I��z�Ƃ��Ĉ������ƂɂȂ�̂Œ��ӂ��K�v
				Float2 SearchDirection[4] =
				{
								Float2(0.0f, 1.0f), //��
								Float2(-1.0f, 0.0f),//��
								Float2(0.0f,-1.0f), //��
								Float2(1.0f, 0.0f)  //�E
				};
};