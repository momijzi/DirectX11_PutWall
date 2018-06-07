class Wall
{
public:
				Wall()
				{
								App::Initialize();
				}
				Wall(Texture* tex)
				{
								for (int i = 0; i < MaxHeight; i++)
								{
												for (int j = 0; j < 6; j++)
												{
																tex->numUV[j] = Float2((float)i, 0.0f);
												}
												//�O���@���̃{�b�N�X
												mesh[i].CreateData(tex,Mesh::CUBEOUT);
								}
								for (int j = 0; j < 4; j++)
								{
												tex->numUV[j] = Float2((float)MaxHeight, 0.0f);
								}
								tex->numUV[4] = Float2((float)MaxHeight + 2, 0.0f);
								tex->numUV[5] = Float2((float)MaxHeight + 1, 0.0f);
								mesh[MaxHeight + 1].CreateData(tex, Mesh::CUBEIN);

								Release();
				}
				~Wall()
				{

				}
				//���ɉ����o����Ă��邩�̔���
				//�����o�������ӏ��̌����Ɏg�������H
				//�����o���Ȃ��ꍇ�����邩������Ȃ��v����
				bool GetPushFlag(int surface, unsigned int width, unsigned int height)
				{
								if (!height < 8)
								{
												//�G���[��
												return false;
								}
									return pushWallFlag[surface][width] & (1 << height);//�����Ƀt���O
				}
				//�����o�������Ɏw�肵���ꏊ��true�ɕϊ�����
				void SetPushFlag(int surface, unsigned int x, unsigned int y)
				{
								if (surface < 4 && y < 8)
								{
												//�G���[��
												return;
								}
								pushWallFlag[surface][x] ^= (1 << y);
				}

				//���݂̈ʒu
				//X�ړ�����.x*length+length/2+!�ړ�����.x*side
				//Ymap[X][Z]&(1<<i)
				//Z�ړ�����.z*length+length/2+!�ړ�����.z*side

				void Wall::PushWallPos(int direction, int side, int height, int pushLength, Float3 *pos, bool &flag)
				{
								if (pos == nullptr)
								{
												return;
								}

								short moveDirection[3] = {};
								for (int i = 0; i < pushLength; i++)
								{
												//�ړ����������߂�
												moveDirection[0] = ((direction + 1) & 1)*(~(direction & 2) + 2);    //moveDirection[0] = (((Direction + 1) % 2)*((Direction / 2) * 2 - 1));
												moveDirection[1] = 0;
												moveDirection[2] = ((direction & 1))*(~(direction & 2) + 2);
												//(((Direction & 1) + 1)*(~(Direction & 2) + 2), 0, ((Direction & 1))*(~(Direction & 2) + 2));

												//�ړ��ꏊ���m�肳����
												pos[i] = Float3(
																(float)(moveDirection[0] * length + (bool)(moveDirection[0]) * (length >> 1) + (bool)(moveDirection[0])*-i + !(bool)(moveDirection[0]) * side),
																(float)(height),
																(float)(moveDirection[2] * (float)length + (bool)(moveDirection[2]) * (length >> 1) + (bool)(moveDirection[2])*-i + !(bool)(moveDirection[2]) * side)
												);
								}

								flag = true;
				}

				//�w�肵���ӏ��̃f�[�^��n���@���݂����Ƀu���b�N�����݂��Ă��邩�̔��f
				bool GetWallData(unsigned int width, unsigned int depth, unsigned int height)
				{
								if (width < length || depth < length)
								{
												//��O����
												return true;
								}
								return map[width][depth] & (1 << height);
				}
				//�w�肵���ӏ��Ƀf�[�^������@�����Ƀu���b�N��ǉ�����
				void Wall::SetWallData(unsigned int width, unsigned int depth, unsigned int height, bool flag)
				{
								if (width < length || depth < length)
								{
												//��O����
								}

								if ((bool)(map[width][depth] & (1 << height)) != flag)
								{
												map[width][depth] ^= (1 << height);
								}
				}
				//�u���b�N�̉����o�����̏���
				void Wall::MoveWall(bool &flag, int direction, int length, int timeCount, Float3* pos)
				{
								if (!flag)
								{
												return;
								}
								//�o����΃^�C�����擾�����������ł�����
								for (int i = 0; i < length; i++)
								{
												pos[i] -= Float3(
																((direction + 1) & 1)*(~(direction & 2) + 2),
																0,
																((direction & 1))*(~(direction & 2) + 2)
												)*((float)1 / 6);//�{����deltaTime���g�������Ƃ���
								}

								if ((float)timeCount / 6 > length)//�R�R��if�������������̂Ō�ŏC��
								{
												flag = false;
												//�R�R�Ŗ������w��ʒu�ɂ��낦��ׂ����Ǝv��
								}
				}

				//������
				void Release()
				{
								//false�ɏ�����
								for (int surface = 0; surface < 4; surface++)
								{
												for (int x = 0; x < length; x++)
												{
																for (int y = 0; y < height; y++)
																{
																				pushWallFlag[surface][x] & (0 << y);
																}
												}
								}
				}
				//�{�b�N�X�̕`��
				void Draw()
				{
								for (int x = 0; x < length; x++)
								{
												for (int z = 0; z < length; z++)
												{
																for (int y = 0; y < height; y++)
																{
																				//bit���������V�t�g���Ă��̏ꏊ�̐��l��flag���f
																				if (map[x][z] & (1 << y))
																				{
																								mesh[y].Draw(Float3((float)x,(float)y,(float)z));
																				}
																}
												}
								}
								mesh[MaxHeight + 1].Draw();
				}
private:

				//���̃Q�[���Ŏg�p�����ő�̃{�b�N�X�̑傫��
				static const unsigned int length = 10;
				//�}�b�v�̍����@�����9�ȏ�ɂ���Ɖ���̂Œ���
				static const unsigned int MaxHeight = 8;
				unsigned int height = 8;

				//�����o�����Ƃ̂ł���ꏊ���ǂ����̔��f�Ɏg�p  2�i���I�ɊǗ�
				char pushWallFlag[4][length] = {};
				//�����o���ꂽ�u���b�N���i�[����@�`��Ɠ����蔻��Ɏg�p
				char map[length][length] = {};

				//�`�悷��{�b�N�X�̃f�[�^���쐬����
				Mesh mesh[MaxHeight + 3] = {};
};