#include"App.hpp"

Wall::Wall()
{
				App::Initialize();
				Texture wallTex(L"texture/StayBox.png");

				//���O�Ɏg�p����e�N�X�`����\�����ʂ𐶐����Ă���
				wall.SetTexture(&wallTex, Float2(2.0f, 1.0f));
				wall.Create();

				//�������y�����邽�߂Ɋe�e�N�X�`����\�����{�b�N�X��p��
				block[0].SetTexture(&wallTex);
				block[0].Create(1);

				block[1].SetTexture(&wallTex);
				block[1].uvData.SetNumUvAll(Float2(1.0f, 0.0f));
				block[1].Create(1);

				block[0].scale = blockSize;
				block[1].scale = blockSize / 2;

				Release();
}

//�\����WallData�̃R���X�g���N�^-----------------------------------------------------------
Wall::WallData::WallData(int surface, int width, int height, int length, float time)
{
				this->surface = surface;
				this->width = width;
				this->height = height;
				this->length = length;
				position = new Float3[length];
				for (int i = 0; i < length; i++)
				{
								position[i] = {};
				}
				flag = false;
				this->time = time;

				//SetInitialPosition(this->surface, this->width, this->height, this->length, *this->position, &this->flag);
}
Wall::WallData::~WallData()
{
				if (position != nullptr)
				{
								delete[] position;
				}
}
//--------------------------------------------------------------------------------------------
bool Wall::GetPushFlag(int surface, unsigned int x, unsigned int y)
{
				if (!(height < 8))
				{
								//�G���[��
								return false;
				}
				return pushWallFlag[surface][x] & (1 << y);//�����Ƀt���O
}

void Wall::SetPushFlag(int surface, unsigned int x, unsigned int y)
{
				if (!(surface < 4) && !(y < 8))
				{
								//�G���[��
								return;
				}
				pushWallFlag[surface][x] ^= (1 << y);
}

bool Wall::GetBlockData(unsigned int width, unsigned int depth, unsigned int height)
{
				if (!(width < length) || !(depth < length))
				{
								//��O����
								return true;
				}
				if (!(height < 8))
				{
								return false;
				}
				return box[width][depth].block & (1 << height);
}

void Wall::SetBlockData(unsigned int width, unsigned int depth, unsigned int height, bool flag)
{
				if (!(width < length) || !(depth < length))
				{
								//��O����
								return;
				}

				if ((bool)(box[width][depth].block & (1 << height)) != flag)
				{
								box[width][depth].block ^= (1 << height);
				}
}

bool Wall::GetPlayerMoveFlag(unsigned int width, unsigned int depth, unsigned int height)
{
				if (!(width < length) || !(depth < length))
				{
								//��O����
								return false;
				}
				return box[width][depth].playerMoveBlock[height];
}
void Wall::SetPlayerMoveFlag(unsigned int width, unsigned int depth, unsigned int height,bool flag)
{
				if (!(width < length) || !(depth < length))
				{
								//��O����
				}
				box[width][depth].playerMoveBlock[height] = flag;
}

void Wall::ResetPlayerMoveFlag()
{
				for(int x = 0; x < length; x++)
				{
								for(int z = 0; z < length; z++)
								{
												for (int y = 0; y < MaxHeight + 1; y++)
												{
																box[x][z].playerMoveBlock[y] = false;
												}
								}
				}
}

//�ړ�������u���b�N�̏����ʒu�̐ݒ�
void Wall::SetInitialPosition(WallData &wallData)
{
				if (wallData.position == nullptr)
				{
								return;
				}
				short moveDirection[3] = {};
				//�ړ����������߂�
				moveDirection[0] = ((wallData.surface + 1) & 1)*(~(wallData.surface & 2) + 2);//moveDirection[0] = (((Direction + 1) % 2)*((Direction / 2) * 2 - 1));
				moveDirection[1] = 0;
				moveDirection[2] = ((wallData.surface & 1))*(~(wallData.surface & 2) + 2);
				//(((Direction & 1) + 1)*(~(Direction & 2) + 2), 0, ((Direction & 1))*(~(Direction & 2) + 2));
				for (int i = 0; i < wallData.length; i++)
				{
								//�ړ��ꏊ���m�肳����
								wallData.position[i] = Float3(
												(float)(moveDirection[0] * (float)length + (float)(moveDirection[0]) *wallData.length*blockSize + (float)(moveDirection[0])*-i*blockSize - moveDirection[0] + !(bool)(moveDirection[0]) * (wallData.width - ((int)length >> 1) + 0.5f)*blockSize),
												(float)((wallData.height + 0.5f)*blockSize),
												(float)(moveDirection[2] * (float)length + (float)(moveDirection[2]) *wallData.length*blockSize + (float)(moveDirection[2])*-i*blockSize - moveDirection[2] + !(bool)(moveDirection[2]) * (wallData.width - ((int)length >> 1) + 0.5f)*blockSize)
								);
				}
				wallData.initPosition = wallData.position[0];

				wallData.flag = true;
}
//�ݒ肵���u���b�N�̈ړ�
void Wall::MoveWall(WallData &wallData)
{
				if (!wallData.flag)
				{
								return;
				}
				for (int i = 0; i < wallData.length; i++)
				{
								wallData.position[i] -= Float3(
												((wallData.surface + 1) & 1)*(~(wallData.surface & 2) + 2),
												0,
												((wallData.surface & 1))*(~(wallData.surface & 2) + 2)
								)*App::GetDeltaTime()*blockSize;
				}
				for (int i = 0; i < wallData.length; i++)
				{
								block[0].scale = blockSize;
								block[0].position = wallData.position[i];
								block[0].Draw();
				}
				if (wallData.time > wallData.length)
				{
								wallData.flag = false;
								short moveDirection[3] = {};
								Float3 *createPos;
								createPos = new Float3[wallData.length];
								//�ړ����������߂�
								moveDirection[0] = ((wallData.surface + 1) & 1)*(~(wallData.surface & 2) + 2);    //moveDirection[0] = (((Direction + 1) % 2)*((Direction / 2) * 2 - 1));
								moveDirection[1] = 0;
								moveDirection[2] = ((wallData.surface & 1))*(~(wallData.surface & 2) + 2);
								//(((Direction & 1) + 1)*(~(Direction & 2) + 2), 0, ((Direction & 1))*(~(Direction & 2) + 2));

								for (int i = 0; i < wallData.length; i++)
								{
												createPos[i] = wallData.initPosition;

												//�ړ��ꏊ���m�肳����
												createPos[i] = Float3((int)createPos[i].x >> 1, (int)createPos[i].y >> 1, (int)createPos[i].z >> 1);

												createPos[i] -= -Float3(moveDirection[0], moveDirection[1], moveDirection[2])*(((int)length - (wallData.length + 1)) >> 1) + Float3(moveDirection[0], 
																moveDirection[1],
																moveDirection[2])*(i + (wallData.length >> 1));
												
												createPos[i] = Float3(((int)length >> 1) * !(bool)moveDirection[0] + createPos[i].x + ((int)wallData.surface >> 1) * (int)length*(bool)moveDirection[0],
																createPos[i].y, 
																((int)length >> 1) * !(bool)moveDirection[2] + createPos[i].z + ((int)wallData.surface >> 1) * (int)length*(bool)moveDirection[2]);

												//createPos[i] += Float3(length*moveDirection[0], 0, length*moveDirection[2]);
												SetBlockData((unsigned int)createPos[i].x, (unsigned int)createPos[i].z, (unsigned int)createPos[i].y, true);
								}
								delete[] createPos;
				}
}

void Wall::Release()
{
				//false�ɏ����� length�͕ǂ̉���
				for (int x = 0; x < length; x++)
				{
								//�ǂ̏�����
								for (int surface = 0; surface < 4; surface++)
								{
												pushWallFlag[surface][x] &= 0;
								}
								//���g�̃u���b�N�̏�����
								for (int z = 0; z < length; z++)
								{
												box[x][z].Release();
								}
				}
				//�����I�ɐݒ肵�Ă���
				/*
				pushWallFlag[3][2] ^= (1 << 1);
				pushWallFlag[3][4] ^= (1 << 2);
				pushWallFlag[3][1] ^= (1 << 4);
				pushWallFlag[3][5] ^= (1 << 3);
				pushWallFlag[3][6] ^= (1 << 0);
				*/
				/*
				box[2][3].block ^= (1 << 0);
				box[1][4].block ^= (1 << 0);
				box[3][3].block ^= (1 << 0);
				box[5][5].block ^= (1 << 0);
				box[2][2].block ^= (1 << 0);
				*/
}

void Wall::Draw()
{
				int halfLength = length >> 1;

				wall.scale = blockSize;
				wall.angles = 0.0f;
				//���̃u���b�N�̕`��
				for (int x = -halfLength; x < halfLength; x++)
				{
								for (int z = -halfLength; z < halfLength; z++)
								{
												for (int y = 0; y < height; y++)
												{
																for (int i = 0; i < 2; i++)
																{
																				//�v���C���[�̍s�����Ƃ̂ł���ꏊ��`��
																				if (GetPlayerMoveFlag(x + halfLength, z + halfLength, y))
																				{
																								block[1].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[1].Draw();
																				}
																				//���ɑ��݂��Ă���u���b�N�𐶐�����
																				//bit���������V�t�g���Ă��̏ꏊ�̐��l��flag���f
																				else if(GetBlockData(x + halfLength, z + halfLength, y))
																				{
																								block[0].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[0].Draw();
																				}
																}
												}
								}
				}
				//�O�ǂ̕`�� �܂�������܂���ƕ`��
				for (int surface = 0; surface < 4; surface++)
				{
								//�`�悷�鏇��  �E�@���@���@��
								wall.angles.y = 90.0f * (-surface + 1);
								//�����O��̃}�b�v�ō쐬
								for (int xz = 0; xz < halfLength; xz++)
								{
												for (int y = 0; y < height; y++)
												{
																for (int i = 0; i < 2; i++)
																{
																				if (!GetPushFlag(surface, xz + (halfLength * i), y))
																				{
																								//length + z�@�Ɓ@length + x���[ x��z��0����Ƃ���
																								wall.position = Float3(
																												(halfLength * wallDrawDirection[surface].x + //x�����Œ�l�̎��Ɏg����
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].y) * blockSize ,//x�����ϓ�����Ƃ��Ɏg�p����
																												y * blockSize + 1.0f,
																												(halfLength * wallDrawDirection[surface].z + //z�����Œ�l�̎��Ɏg����
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].w) * blockSize);//z�����Œ�l�̎��Ɏg����
																								wall.Draw();
																				}
																}
												}
								}
				}
				//�Ō�ɒ�ʂ̕`��
				wall.position = Float3(0.0f, -0.5f * blockSize + 1.0f, 0.0f);
				wall.angles = Float3(90.0f, 0.0f, 0.0f);
				wall.scale = Float3(length, length, 1.0f) * blockSize;
				wall.Draw();
}