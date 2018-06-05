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
												return;
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

				bool GetWallData(unsigned int width, unsigned int depth, unsigned int height)
				{
								if (!height < 8)
								{
												//�ŏ㕔�Ȃ��߃u���b�N�͑��݂��Ȃ�
												return false;
								}
								return map[width][depth] & (1 << height);
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