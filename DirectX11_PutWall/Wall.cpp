#include"App.hpp"

Wall::Wall()
{
				App::Initialize();
				wallTex.Load(L"texture/StayBox.png");
				wallTex.SetDivide(Float2(4.0f, 1.0f));

				//���O�Ɏg�p����e�N�X�`����\�����ʂ𐶐����Ă���
				wall[0].Create(&wallTex);

				//�������y�����邽�߂Ɋe�e�N�X�`����\�����{�b�N�X��p��
				block[0].Create(&wallTex,1);

				wallTex.SetNumUvAll(Float2(1.0f, 0.0f));
				wall[1].Create(&wallTex);
				
				wallTex.SetNumUvAll(Float2(2.0f, 0.0f));
				wall[2].Create(&wallTex);

				wallTex.SetNumUvAll(Float2(3.0f, 0.0f));
				block[1].Create(&wallTex, 1);

				wall[0].scale = blockSize;
				wall[1].scale = blockSize;
				wall[2].scale = blockSize;
				wall[2].angles.x = 90.0f;

				block[0].scale = blockSize;
				block[1].scale = blockSize;

				Release();
}

//�\����WallData�̃R���X�g���N�^-----------------------------------------------------------
void Wall::WallData::SetWallData(int surface, int width, int height, float time)
{
				this->surface = surface;
				this->width = width;
				this->height = height;
				moveFlag = false;
				drawTexFlag = false;
				checkLengthFlag = false;
				this->time = time;
				Float3 moveDirection = 0.0f;
				//SetInitialPosition(this->surface, this->width, this->height, this->length, *this->position, &this->flag);
}

//--------------------------------------------------------------------------------------------
bool Wall::GetPushFlag(int surface, int x, int y)
{
				if (!(height < 8))
				{
								//�G���[��
								return false;
				}
								return pushWallFlag[surface][x] & (1 << y);//�����Ƀt���O
}

void Wall::SetPushFlag(int surface, int width, int height,bool flag)
{
				if (!(surface < 4) && !(height < 8))
				{
								//�G���[��
								return;
				}
				if ((bool)(pushWallFlag[surface][width] & (1 << height)) != flag)
				{
								pushWallFlag[surface][width] ^= (1 << height);
				}
}

bool Wall::GetBlockData(int width, int depth, int height)
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

void Wall::SetBlockData(int width, int depth, int height, bool flag)
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

bool Wall::GetPlayerMoveFlag(int width, int depth, int height)
{
				if (!(width < length) || !(depth < length))
				{
								//��O����
								return false;
				}
				if (height > this->height)
				{
								return true;
				}
				return box[width][depth].playerMoveBlock[height];
}
void Wall::SetPlayerMoveFlag(int width, int depth, int height,bool flag)
{
				if (width >= length || depth >= length || width < 0 || depth < 0)
				{
								//��O����
								return;
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

void Wall::SelectLookWall(float height, float angleY)
{
				//�Ίp���Ŕ��肷��̂�+135
				if (angleY < 0)
				{
								angleY += ((-(int)angleY / 360) + 1) * 360;
				}
				wallData.surface = (360 - ((int)angleY + 225) % 360) / 90;
				wallData.width = length / (int)blockSize;
				wallData.height = (int)height / (int)blockSize;
}

void Wall::SelectToWall(int moveDirection)
{
				//�����̕ύX�����ɂ͖��ʂ�����\��������C�����܂�
				//���������Z�k�ł���C������
				if (moveDirection != 0)
				{
								if (wallData.drawTexFlag)
								{
												SetPushFlag(wallData.surface, wallData.width, wallData.height, false);
								}

								wallData.height -= ((moveDirection & 2)*(moveDirection & 1)) - (moveDirection & 1);
								wallData.width -= ((((moveDirection - 1) & 2)*((moveDirection - 1) & 1)) - ((moveDirection - 1) & 1)) *
												-(((((wallData.surface + 1) & 3) >> 1) << 1) - 1);

								if (wallData.width < 0)
								{
												wallData.width = (!(bool)(wallData.surface & 2))*(length - 1);
												wallData.surface += ((((wallData.surface + 1) & 3) >> 1) << 1) - 1;
								}
								else if (wallData.width >= length)
								{
												wallData.width = (!(bool)(wallData.surface & 2))*(length - 1);
												wallData.surface -= ((((wallData.surface + 1) & 3) >> 1) << 1) - 1;
								}

								if (wallData.surface < 0)
								{
												wallData.surface = 3;
								}
								else if (wallData.surface > 3)
								{
												wallData.surface = 0;
								}
								if (wallData.height < 0)
								{
												wallData.height = 0;
								}
								else if (wallData.height > this->height - 1)
								{
												wallData.height = this->height - 1;
								}
				}
				//GetPushFlag�͂����ɉ����o����ꏊ�����݂��ĂȂ������Ƃ���true��Ԃ��֐��̂��ߔ��]������
				wallData.drawTexFlag = !GetPushFlag(wallData.surface, wallData.width, wallData.height);
				if (wallData.drawTexFlag != false)
				{
								SetInitialPosition();
								SetPushFlag(wallData.surface, wallData.width, wallData.height, true);
				}
}
void Wall::MoveDirectionUpdate()
{
				wallData.moveDirection.x = ((wallData.surface + 1) & 1)*(~(wallData.surface & 2) + 2);//moveDirection[0] = (((Direction + 1) % 2)*((Direction / 2) * 2 - 1));
				wallData.moveDirection.y = 0;//���͏c�Ƀu���b�N��L�΂����Ƃ���������0���i�[���Ă���
				wallData.moveDirection.z = ((wallData.surface & 1))*(~(wallData.surface & 2) + 2);
}

//�ړ�������u���b�N�̏����ʒu�̐ݒ�
void Wall::SetInitialPosition()
{
				//�u���b�N�������o���ӏ����i�[
				//����͔{�������̂ŕ`��Ŕ{����������悤��
				wallData.createInitPosition = Float3(
								((length >> 1) * wallDrawDirection[wallData.surface].x + //x�����Œ�l�̎��Ɏg����
								(wallData.width - (length >> 1) + 0.5f) * wallDrawDirection[wallData.surface].y),//x�����ϓ�����Ƃ��Ɏg�p����
								
								wallData.height + 0.5f,
								
								(length >> 1) * wallDrawDirection[wallData.surface].z + //z�����Œ�l�̎��Ɏg����
								((wallData.width - (length >> 1) + 0.5f) * wallDrawDirection[wallData.surface].w));

				//�����ŖʂƂ��Ă̍��W�����Ƃ̍��W�ɖ߂�
				//�܂��ϓ�������𐳋K�ɖ߂��܂�
				wallData.setInitiPosition = wallData.createInitPosition + Float3(wallDrawDirection[wallData.surface].y,
												0.0f, wallDrawDirection[wallData.surface].w) * ((length >> 1) - 0.5f);
				//���ɕϓ����Ȃ����𐳋K�ɒ����܂�
				//�܂��ǂ���������Ɋ񂹂�
				wallData.setInitiPosition += Float3(wallDrawDirection[wallData.surface].x, 0.0f,
								wallDrawDirection[wallData.surface].z) * 0.5f;
				//���̂��ƒ��S�Ɋ񂹂邱�ƂŌ덷�𖳂���
				//-0.5f�͊��ɂ��炵�Ă���̂Ō덷������
				wallData.setInitiPosition += Float3(fabs(wallDrawDirection[wallData.surface].x), 0.0f,
								fabs(wallDrawDirection[wallData.surface].z)) * ((length >> 1) - 0.5f);
				//y�����Œ�l�ł���Ă���̂ł��炵�܂�
				wallData.setInitiPosition.y -= 0.5f;
}
void Wall::SetPushWallLength(int addLength,Float3 playerPos1,Float3 playerPos2)
{
				if(addLength > 0)
				{
								if (wallData.length >= length)
								{
												return;
								}
								Float3 chackPos = wallData.setInitiPosition - wallData.moveDirection * (wallData.length + addLength);
								if (GetBlockData((int)chackPos.x, (int)chackPos.z, (int)chackPos.y) ||
												App::SameChackFloat3(chackPos, playerPos1) || App::SameChackFloat3(chackPos, playerPos2))
								{
												return;
								}
				}
				else
				{
								if (wallData.length == 1)
								{
												return;
								}
				}
				wallData.length += addLength;
}

//�ݒ肵���u���b�N�̈ړ�
void Wall::MoveWall()
{
				if (!wallData.moveFlag)
				{
								return;
				}
				wallData.time += App::GetDeltaTime();
				
				if (wallData.time > wallData.length)
				{
								wallData.moveFlag = false;

								for (int i = 1; i <= wallData.length; i++)
								{
												//+1�����闝�R��initPosition���{�b�N�X�̂P�O�̏ꏊ�̂��ߊi�[�̂��߂ɂɂ��炷
												//initPosition�̂������ς��Ă�����
												wallData.setInitiPosition -= wallData.moveDirection;

												SetBlockData((int)wallData.setInitiPosition.x , (int)wallData.setInitiPosition.z, 
																(int)wallData.setInitiPosition.y, true);
								}
								wallData.moveFlag = false;
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

				box[2][3].block ^= (1 << 0);
				box[1][4].block ^= (1 << 0);
				box[3][3].block ^= (1 << 0);
				box[5][5].block ^= (1 << 0);
				box[2][2].block ^= (1 << 0);
				*/
}

void Wall::Draw(bool playerMovePosDrawFlag)
{
				int halfLength = length >> 1;

				if (wallData.drawTexFlag)
				{
								wall[1].angles.y = 90.0f * (-wallData.surface + 1);
								//�����I�ɍ��W�ς��܂�
								wall[1].position = wallData.createInitPosition * blockSize;
								wall[1].Draw();
				}
				//�����o���ꏊ��`�悷��//����𕪂��Ă��闝�R�͎n�܂�n�_���܂������Ⴄ����
				if (wallData.checkLengthFlag)
				{
								for (int i = 1; i <= wallData.length; i++)
								{
												block[1].position = (wallData.createInitPosition - wallData.moveDirection * (-0.5f + i)) * blockSize;
												block[1].Draw();
								}
				}
				//�����Ă���Œ��̃u���b�N�̕`��
				else if (wallData.moveFlag)
				{
								for (int i = 0; i < wallData.length; i++)
								{
												block[0].position = (wallData.createInitPosition + wallData.moveDirection * (i - wallData.time + 0.5f))* blockSize;
												block[0].Draw();
								}
				}

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
																				if (GetPlayerMoveFlag(x + halfLength, z + halfLength, y) && playerMovePosDrawFlag)
																				{
																								block[1].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[1].Draw();
																				}
																				//���ɑ��݂��Ă���u���b�N�𐶐�����
																				//bit���������V�t�g���Ă��̏ꏊ�̐��l��flag���f
																				else if (GetBlockData(x + halfLength, z + halfLength, y))
																				{
																								block[0].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[0].Draw();
																				}
																}
												}
												wall[2].position = Float3(x + 0.5f, 0, z + 0.5f) * blockSize;
												wall[2].Draw();
								}
				}

				wall[0].angles.x = 0.0f;

				//�O�ǂ̕`�� �܂�������܂���ƕ`��
				for (int surface = 0; surface < 4; surface++)
				{
								//�`�悷�鏇��  �E�@���@���@��
								wall[0].angles.y = 90.0f * (-surface + 1);
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
																								wall[0].position = Float3(
																												(halfLength * wallDrawDirection[surface].x + //x�����Œ�l�̎��Ɏg����
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].y) * blockSize ,//x�����ϓ�����Ƃ��Ɏg�p����
																												y * blockSize + 1.0f,
																												(halfLength * wallDrawDirection[surface].z + //z�����Œ�l�̎��Ɏg����
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].w) * blockSize);//z�����Œ�l�̎��Ɏg����
																								wall[0].Draw();
																				}
																}
												}
								}
				}
}


/*

0 (1,0,0)
1 (0,0,1)
2 (-1,0,0)
3 (0,0,-1)

*/

/*
* (float)length +//�E�[
wallData.length	�����o���ő�̒���																				* blockSize+
 -i *�@���Ԗڂ�length���@blockSize + 
!(bool)(wallData.moveDirection[0]) *�@(wallData.width - ((int)length >> 1) + 0.5f)*blockSize),
*/
