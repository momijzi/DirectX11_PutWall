#include"App.hpp"

Wall::Wall(Texture* tex)
{
				//���O�Ɏg�p����e�N�X�`����\�����ʂ𐶐����Ă���
				wall.Create(tex);
				block.Create(tex,1);

				Release();
}

bool Wall::GetPushFlag(int surface, unsigned int width, unsigned int height)
{
				if (!height < 8)
				{
								//�G���[��
								return false;
				}
				return pushWallFlag[surface][width] & (1 << height);//�����Ƀt���O
}

void Wall::SetPushFlag(int surface, unsigned int x, unsigned int y)
{
				if (surface < 4 && y < 8)
				{
								//�G���[��
								return;
				}
				pushWallFlag[surface][x] ^= (1 << y);
}

void Wall::SetInitialPosition(int direction, int side, int height, int pushLength, Float3 *createWallPos, bool &flag)
{
				if (createWallPos == nullptr)
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
								createWallPos[i] = Float3(
												(float)(moveDirection[0] * length + (bool)(moveDirection[0]) * (length >> 1) + (bool)(moveDirection[0])*-i + !(bool)(moveDirection[0]) * side),
												(float)(height),
												(float)(moveDirection[2] * (float)length + (bool)(moveDirection[2]) * (length >> 1) + (bool)(moveDirection[2])*-i + !(bool)(moveDirection[2]) * side)
								);
				}
				flag = true;
}

bool Wall::GetWallData(unsigned int width, unsigned int depth, unsigned int height)
{
				if (width < length || depth < length)
				{
								//��O����
								return true;
				}
				return box[width][depth] & (1 << height);
}

void Wall::SetWallData(unsigned int width, unsigned int depth, unsigned int height, bool flag)
{
				if (width < length || depth < length)
				{
								//��O����
				}

				if ((bool)(box[width][depth] & (1 << height)) != flag)
				{
								box[width][depth] ^= (1 << height);
				}
}

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

void Wall::Release()
{
				//false�ɏ����� length�͕ǂ̉���
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

void Wall::Draw()
{
				int halfLength = length / 2;
				wall.scale = blockSize;
				wall.angles = 0.0f;
				//for (int x = 0; x < length; x++)
				//{
				//				for (int z = 0; z < length; z++)
				//				{
				//								for (int y = 0; y < height; y++)
				//								{
				//												//bit���������V�t�g���Ă��̏ꏊ�̐��l��flag���f
				//												if (box[x][z] & (1 << y))
				//												{
				//																wall[y].position = Float3((float)x, (float)y, (float)z);
				//																wall[y].Draw();
				//												}
				//								}
				//				}
				//}
				//�O�ǂ̕`�� �܂�������܂���ƕ`��
				for (int i = 0; i < 4; i++)
				{
								//�`�悷�鏇��  �E�@���@���@��
								wall.angles.y = 90.0f * (i + 1);
								//�����O��̃}�b�v�ō쐬
								for (int xz = 0; xz < halfLength; xz++)
								{
												for (int y = 0; y < height; y++)
												{
																for (int sign = -1; sign < 2; sign += 2)
																{
																				if (!GetPushFlag(i, halfLength + (xz * sign), y))
																				{
																								//length + z�@�Ɓ@length + x���[ x��z��0����Ƃ���
																								wall.position = Float3(
																												halfLength * blockSize * wallDrawDir[i].x + ((xz * blockSize + blockSize/2) * sign) * wallDrawDir[i].y,
																												y * blockSize + 1.0f,
																												halfLength * blockSize * wallDrawDir[i].z + ((xz * blockSize + blockSize/2) * sign) * wallDrawDir[i].w);
																								wall.Draw();
																			}
																}
												}
								}
				}
				//�Ō�ɒ�ʂ̕`��
				wall.position = Float3(0.0f, -0.5f * blockSize + 1.0f, 0.0f);
				wall.angles = Float3(90.0f,0.0f,0.0f);
				wall.scale = Float3(length, length, 1.0f) * blockSize;
				wall.Draw();
}