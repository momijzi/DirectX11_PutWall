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
				//��U�̏o��������u���b�N��FIRST,��U��SECOND
				//ERROR��,�\��O�̐��l��Ԃ����Ƃ��ɕԂ�
				enum BlockType {NORMAL,FIRST,SECOND,NON = 98,ERRORNUM = 99};

				//�u���b�N�̊�{�T�C�Y
				const float blockSize = 2.0f;
				//���̃Q�[���Ŏg�p�����ő�̃{�b�N�X�̑傫��
				static const int MaxLength = 8;
				//�}�b�v�̍����@�����9�ȏ�ɂ���Ɖ���̂Œ���
				static const int MaxHeight = 8;
				//���݂̃Q�[���N���A�����̍���
				unsigned int height = 8;

				struct WallData
				{
								WallData() {};

								void ResetWallData(int surface = 0, int width = 0, int height = 0, float time = 0.0f);
								//�����o���ʁA�ꏊ�A�����̃f�[�^�A�����o������
								int surface = 0;
								int width = 0;
								int height = 0;
								int length = 1;
								//����`�悷�邩�B�����o���ꏊ�A�����o���n�_�A�����o���̕`��A�����o���̎��ԁA�����o���u���b�N�̎��
								int drawTexFlag = 0;
								bool checkLengthFlag = false;
								bool moveFlag = false;
								float time = 0.0f;
								BlockType pushBlockType[Wall::MaxLength];

								Float3 moveDirection = 0.0f;
								//�ʒu���m�肵���Ƃ��ɉ����o���ďo�Ă���ŏ��̃u���b�N�̈ʒu������
								//����͂܂��ʒu�����肵�Ă��Ȃ��u���b�N�̕`��Ɏg�p����
								Float3 createInitPosition = Float3(0.0f, 0.0f, 0.0f);
								//����͉����o���ʒu�����܂������Ƀf�[�^�i�[�p�f�[�^
								Float3 setInitiPosition = Float3(0.0f, 0.0f, 0.0f);
				};
				WallData wallData;

				struct BoxData
				{
								//�u���b�N�����݂��Ă��邩
								BlockType blockType[Wall::MaxHeight];
								//�v���C���[���ړ��\�ȏꏊ// +1�̗��R��8�i�̍����������Ă��̏オ�N���A�̍����Ȃ���
								bool playerMoveBlock[MaxHeight + 1];

								void Release()
								{
												for (int i = 0; i < MaxHeight; i++)
												{
																blockType[i] = NON;
																playerMoveBlock[i] = false;
												}
												playerMoveBlock[MaxHeight] = false;
								}
				};

				//4���ʂ̕ǂ��猻�݃u���b�N���o�Ă��邩�̔��f�Ɏg�p����
				bool GetPushFlag(int surface, int width, int height);
				//�����o�������Ɏw�肵���ꏊ��true�ɕϊ�����
				void SetPushFlag(int surface, int width, int height, bool flag);

				//BoxData��block�̎w�肵���f�[�^���Ăяo���܂��͐ݒ肷��
				BlockType GetBlockData(int width, int depth, int height);
				void SetBlockData(int width, int depth, int height, BlockType blockType);
				
				//BoxData��playerMoveFlag�̎w�肵���f�[�^���Ăяo���܂��͐ݒ肷��
				bool GetPlayerMoveFlag(int width, int depth, int height);
				void SetPlayerMoveFlag(int width, int depth, int height,bool flag);
				//���̃^�[���ɂȂ������Ɏ��̃v���C���[�̈ړ��ꏊ��ݒ肷�邽�߂ɏ�����
				void ResetPlayerMoveFlag();
				//�����̑I��
				void SelectLookWall(float height, float angleY);
				//�\���L�[�ł̑I��
				void SelectToWall(int moveDirection, float playerPosY, float nextPlayerPosY);
				//�����o���ꏊ��ݒ肵���Ƃ��ɉ����o���������o��
				void MoveDirectionUpdate();
				//�����o���ǂ̏����n�_�̐ݒ�
				void SetInitialPosition();
				//�u���b�N�̉����o���ʂ̒����Ɏg�p
				void SetPushWallLength(int addlength,Float3 playerPos1 = (-1.0f, -1.0f, -1.0f),
								Float3 playerPos2 = (-1.0f, -1.0f, -1.0f), bool playerTurn = true);
				//�u���b�N�̏�ԕύX
				void ChangePushWallLine();
				//�u���b�N�̉����o�����̏���
				void MoveWall();

				//�������֐�
				void Release();
				
				//�{�b�N�X�̕`��
				void Draw(bool playerMovePosDrawFlag, float playerPosY, float nextPlayerPosY);
				
private:
				Texture wallTex;

				//�����o�����Ƃ̂ł���ꏊ���ǂ����̔��f�Ɏg�p  2�i���I�ɊǗ�
				char pushWallFlag[4][MaxLength] = {};
				//�u���b�N���l�܂��Ă����ꏊ�@�u���b�N������ӏ���true�Ƃ���
				BoxData box[MaxLength][MaxLength] = {};
				//�`�悷��f�[�^���쐬����//�e�N�X�`�����쐬�i����͏��������������邽�߂Ɂj
				Plane wall[5];
				Cube block[5];

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

