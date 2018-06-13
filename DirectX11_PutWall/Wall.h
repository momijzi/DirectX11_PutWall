#pragma once
class Wall
{
public:
				Wall()
				{
								App::Initialize();
				}
				Wall(Texture* tex);
				
				~Wall()
				{

				}

				struct WallData
				{
								WallData(int surface, int width, int height, int length, float time);
								
								~WallData();
								
								int surface = 0;
								int width = 0;
								int height = 0;
								int length = 0;
								Float3 *position;
								Float3 initPosition = Float3(0, 0, 0);
								bool flag = false;
								float time = 0.0f;
				};

				/*
				struct WallData2
				{
								Float3 position;//�����o���u���b�N�̐擪���W
								int length;//�����o������
								int pushDirection;//�����o������
								bool activeFlag;//���݉����o�����������Ă��邩
								Float2 Direction[4] = {
												Float2(-1.0f, 0.0f),//��
												Float2(0.0f, 1.0f), //�O
												Float2(1.0f, 0.0f), //�E
												Float2(0.0f, -1.0f)	//��
								};
				};
				void CreateBlock();
				*/

				//4���ʂ̕ǂ��猻�݃u���b�N���o�Ă��邩�̔��f�Ɏg�p����
				bool GetPushFlag(int surface, unsigned int x, unsigned int y);
				//�����o�������Ɏw�肵���ꏊ��true�ɕϊ�����
				void SetPushFlag(int surface, unsigned int x, unsigned int y);
				//�w�肵���ӏ��̃f�[�^��n���@���݂����Ƀu���b�N�����݂��Ă��邩�̔��f
				bool GetBoxData(unsigned int width, unsigned int depth, unsigned int height);
				//�w�肵���ӏ��Ƀf�[�^������@�����Ƀu���b�N��ǉ����� ��ɉ����o�����u���b�N���Î~�����Ƃ��ɂ��̏ꏊ��true�����܂���
				void SetBoxData(unsigned int width, unsigned int depth, unsigned int height, bool flag);
				
				//�����o���ǂ̏����n�_�̐ݒ�
				void SetInitialPosition(WallData &wallData);
				//�u���b�N�̉����o�����̏���
				void MoveWall(WallData &wallData);

				//�������֐�
				void Release();
				
				//�{�b�N�X�̕`��
				void Draw();
				
private:
				//�u���b�N�̊�{�T�C�Y
				const float blockSize = 2.0f;
				//���̃Q�[���Ŏg�p�����ő�̃{�b�N�X�̑傫��
				static const unsigned int length = 8;
				//�}�b�v�̍����@�����9�ȏ�ɂ���Ɖ���̂Œ���
				static const unsigned int MaxHeight = 8;
				unsigned int height = 5;
				//�����o�����Ƃ̂ł���ꏊ���ǂ����̔��f�Ɏg�p  2�i���I�ɊǗ�
				char pushWallFlag[4][length] = {};
				//�u���b�N���l�܂��Ă����ꏊ�@�u���b�N������ӏ���true�Ƃ���
				char box[length][length] = {};
				//�`�悷��f�[�^���쐬����//�e�N�X�`�����쐬�i����͏��������������邽�߂Ɂj
				Plane wall;
				Cube block;
				//�ǂ̕`��ł��̂S�����ŕ`��̎d�����ς��̂ł��̕ύX�p
				Float4 wallDrawDir[4] =
				{
								Float4(1.0f,0.0f,0.0f,1.0f),				//�E
								Float4(0.0f,1.0f,1.0f,0.0f),					//��
								Float4(-1.0f,0.0f,0.0f,1.0f),			//��
								Float4(0.0f,1.0f,-1.0f,0.0f)			//��
				};
};

