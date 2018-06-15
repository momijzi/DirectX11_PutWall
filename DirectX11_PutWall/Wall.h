#pragma once

/*
�ǂ̕`��A���̃u���b�N�̕`��A�����o���u���b�N�̕`��Ə�������ɍs��
�������������������ق��������C������
�܂�blockSize��length���g���ʒu����������̂�
������ǂ��ɂ����܂��g���������c�c
*/

class Wall
{
public:
				Wall();
				~Wall() {};
				
				//�u���b�N�̊�{�T�C�Y
				const float blockSize = 2.0f;
				//���̃Q�[���Ŏg�p�����ő�̃{�b�N�X�̑傫��
				static const unsigned int length = 8;
				//�}�b�v�̍����@�����9�ȏ�ɂ���Ɖ���̂Œ���
				static const unsigned int MaxHeight = 8;
				//���݂̃Q�[���N���A�����̍���
				unsigned int height = 5;

				struct WallData
				{
								WallData() {};

								void SetWallData(int surface = 0, int width = 0, int height = 0, float time = 0.0f);
								
								int surface = 0;
								int width = 0;
								int height = 0;
								int length = 1;
								//�ʒu���m�肵���Ƃ��ɉ����o���ďo�Ă���ŏ��̃u���b�N�̈ʒu������
								Float3 initPosition = Float3(0, 0, 0);
								//MoveWall�Ŏ�Ɏg�p �ړ�������������false �܂��`��ł��g�p
								bool moveFlag = false;
								float time = 0.0f;
								//���̃f�[�^�����ݑ��݂��Ă��邩 false�ő��݂��Ă���E�E������
								bool drawTexFlag = true;
								bool checkLengthFlag = false;
								short moveDirection[3] = {};
				};
				WallData wallData;

				struct BoxData
				{
								//�u���b�N�����݂��Ă��邩
								char block;
								//�v���C���[���ړ��\�ȏꏊ// +1�̗��R��8�i�̍����������Ă��̏オ�N���A�̍����Ȃ���
								bool playerMoveBlock[MaxHeight + 1];

								void Release()
								{
												block &= 0;
												for (int i = 0; i < MaxHeight + 1; i++)
												{
																playerMoveBlock[i] = false;
												}
								}
				};

				//4���ʂ̕ǂ��猻�݃u���b�N���o�Ă��邩�̔��f�Ɏg�p����
				bool GetPushFlag(int surface, unsigned int width, unsigned int height);
				//�����o�������Ɏw�肵���ꏊ��true�ɕϊ�����
				void SetPushFlag(int surface, unsigned int width, unsigned int height, bool flag);

				//BoxData��block�̎w�肵���f�[�^���Ăяo���܂��͐ݒ肷��
				bool GetBlockData(unsigned int width, unsigned int depth, unsigned int height);
				void SetBlockData(unsigned int width, unsigned int depth, unsigned int height, bool flag);
				
				//BoxData��playerMoveFlag�̎w�肵���f�[�^���Ăяo���܂��͐ݒ肷��
				bool GetPlayerMoveFlag(unsigned int width, unsigned int depth, unsigned int height);
				void SetPlayerMoveFlag(unsigned int width, unsigned int depth, unsigned int height,bool flag);
				
				//���̃^�[���ɂȂ������Ɏ��̃v���C���[�̈ړ��ꏊ��ݒ肷�邽�߂ɏ�����
				void ResetPlayerMoveFlag();

				//�����̑I��
				void SelectLookWall(float height, float angleY);
				//�\���L�[�ł̑I��
				void SelectToWall(int moveDirection = 0);
		
				void MoveDirectionUpdate();

				Float3 GetWallSelectPosition();

				//�����o���ǂ̏����n�_�̐ݒ�
				void SetInitialPosition();
				//�u���b�N�̉����o�����̏���
				void MoveWall();

				//�������֐�
				void Release();
				
				//�{�b�N�X�̕`��
				void Draw(bool playerMovePosDrawFlag);
				
private:
				Texture wallTex;

				//�����o�����Ƃ̂ł���ꏊ���ǂ����̔��f�Ɏg�p  2�i���I�ɊǗ�
				char pushWallFlag[4][length] = {};
				//�u���b�N���l�܂��Ă����ꏊ�@�u���b�N������ӏ���true�Ƃ���
				BoxData box[length][length] = {};
				//�`�悷��f�[�^���쐬����//�e�N�X�`�����쐬�i����͏��������������邽�߂Ɂj
				Plane wall[2];
				Cube block[2];

				WallData pushWallData;
				//�ǂ̕`��ł��̂S�����ŕ`��̎d�����ς��̂ł��̕ύX�p
				Float4 wallDrawDirection[4] =
				{
								Float4( 1.0f, 0.0f, 0.0f, 1.0f),//�E
								Float4( 0.0f, 1.0f, 1.0f, 0.0f),//��
								Float4(-1.0f, 0.0f, 0.0f, 1.0f),//��
								Float4( 0.0f, 1.0f,-1.0f, 0.0f)	//��
				};
				/*
				//�ǂ̕`��͕��ԕ����ɂ������������̂ł͂Ȃ�����
				//�ǂ�I�����鎞�Ɍ��݂̕������Ƃ��Ȃ�߂�ǂ��������������Ȃ���΂����Ȃ�
				//�����C�����悤�ɂ����݂̕����Ői�߂Ă������ߔ��ɕς���̂����Ԃ�������
				//�����Ƃ��Ă͖��Ȃ��̂Ō��݂̏�ԂŐi�s
				//���Ԃ��ł������ɕ��ԕ����ɕς��Ă�������
				Float4 wallDrawDirection[4] =
				{
								Float4(1.0f, 0.0f, 0.0f, 1.0f),//�E
								Float4(0.0f, 1.0f, 1.0f, 0.0f),//��
								Float4(-1.0f, 0.0f, 0.0f, 1.0f),//��
								Float4(0.0f, 1.0f,-1.0f, 0.0f)	//��
				}*/
};

