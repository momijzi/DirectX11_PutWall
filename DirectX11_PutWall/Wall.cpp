#include"App.hpp"

Wall::Wall()
{
				App::Initialize();
				wallTex.Load(L"texture/wallTexture_pw.png");
				wallTex.SetDivide(Float2(5.0f, 1.0f));
				blockTex.Load(L"texture/blockTexture_pw.png");
				blockTex.SetDivide(Float2(4.0f, 1.0f));

				//���O�Ɏg�p����e�N�X�`����\�����ʂ𐶐����Ă���
				//��������X�y�����邽��
				//�ǂ̃e�N�X�`����ݒ�
				for (int i = 0; i < 5; i++)
				{
								wallTex.SetNumUvAll(Float2((float)i, 0.0f));
								wall[i].Create(&wallTex);

								wall[i].scale = blockSize;
				}
				//�u���b�N�̃e�N�X�`����ݒ�
				for (int i = 0; i < 3; i++)
				{
								blockTex.SetNumUvAll(Float2((float)i, 0.0f));
								block[i].Create(&blockTex, 1);

								blockTex.SetNumUv(Float2(3.0f, 0.0f), 4);
								block[i + 3].Create(&blockTex, 1);

								block[i].scale = block[i + 3].scale = blockSize;
				}
				Release();
}

//�\����WallData�̃R���X�g���N�^-----------------------------------------------------------
void Wall::WallData::ResetWallData(int surface, int width, int height, float time)
{
				this->surface = surface;
				this->width = width;
				this->height = height;
				moveFlag = false;
				drawTexFlag = false;
				checkLengthFlag = false;
				this->time = time;
				Float3 moveDirection = 0.0f;
				for (int i = 0; i < Wall::MaxLength; i++)
				{
								pushBlockType[i] = NORMAL;
				}
}

//--------------------------------------------------------------------------------------------
void Wall::ResetBlockType()
{
				for (int x = 0; x < MaxLength; x++)
				{
								for (int z = 0; z < MaxLength; z++)
								{
												for (int y = 0; y < MaxHeight; y++)
												{
																box[x][z].blockType[y] = NON;
												}
								}
				}
}
void Wall::ResetPlayerMoveFlag()
{
				for (int x = 0; x < MaxLength; x++)
				{
								for (int z = 0; z < MaxLength; z++)
								{
												for (int y = 0; y < MaxHeight; y++)
												{
																box[x][z].playerMoveBlock[y] = false;
												}
								}
				}
}

Wall::BlockType Wall::GetBlockData(int width, int depth, int height)
{
				if (!(width < MaxLength) || !(depth < MaxLength) || width < 0 || depth < 0)
				{
								//��O����
								return ERRORNUM;
				}
				if (height >= this->MaxHeight)
				{
								return NON;
				}
				return box[width][depth].blockType[height];
}

void Wall::SetBlockData(int width, int depth, int height, BlockType blockType)
{
				if (!(width < MaxLength) || !(depth < MaxLength) || height >= this->MaxHeight)
				{
								//��O����
								return;
				}
				box[width][depth].blockType[height] = blockType;
}

bool Wall::GetPlayerMoveFlag(int width, int depth, int height)
{
				if (!(width < MaxLength) || !(depth < MaxLength) || width < 0 || depth < 0 || height > this->MaxHeight)
				{
								//��O����
								return false;
				}
				return box[width][depth].playerMoveBlock[height];
}
void Wall::SetPlayerMoveFlag(int width, int depth, int height,bool flag)
{
				if (width >= MaxLength || depth >= MaxLength || width < 0 || depth < 0 || height > this->MaxHeight)
				{
								//��O����
								return;
				}
				box[width][depth].playerMoveBlock[height] = flag;
}

void Wall::SelectLookWall(float playerHeight, float angleY)
{
				//�Ίp���Ŕ��肷��̂�+135
				if (angleY < 0)
				{
								angleY += ((-(int)angleY / 360) + 1) * 360;
				}
				wallData.surface = (360 - ((int)angleY + 225) % 360) / 90;
				wallData.width = MaxLength / (int)blockSize;
				wallData.height = (int)playerHeight;
}

void Wall::SelectToWall(int moveDirection, float playerPosY, float nextPlayerPosY)
{
				wallData.height -= ((moveDirection & 2)*(moveDirection & 1)) - (moveDirection & 1);
				wallData.width -= ((((moveDirection - 1) & 2)*((moveDirection - 1) & 1)) - ((moveDirection - 1) & 1)) *
								-(((((wallData.surface + 1) & 3) >> 1) << 1) - 1);

				if (wallData.width < 0)
				{
								wallData.width = (!(bool)(wallData.surface & 2))*(MaxLength - 1);
								wallData.surface += ((((wallData.surface + 1) & 3) >> 1) << 1) - 1;
				}
				else if (wallData.width >= MaxLength)
				{
								wallData.width = (!(bool)(wallData.surface & 2))*(MaxLength - 1);
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
				else if (wallData.height > this->MaxHeight - 1)
				{
								wallData.height = this->MaxHeight - 1;
				}
				SetInitialPosition();
				//�����o�����Ƃ��\�ȍ������ǂ���
				if ((playerPosY + 1 >= wallData.height || nextPlayerPosY + 1 >= wallData.height) &&
								playerPosY - 1 <= wallData.height)
				{
								//�����o����Ƀf�[�^�����݂��邩
								wallData.drawTexFlag = 2;
								return;
				}
				wallData.drawTexFlag = 1;
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
								((MaxLength >> 1) * wallDrawDirection[wallData.surface].x + //x�����Œ�l�̎��Ɏg����
								(wallData.width - (MaxLength >> 1) + 0.5f) * wallDrawDirection[wallData.surface].y),//x�����ϓ�����Ƃ��Ɏg�p����
								
								wallData.height + 0.5f,
								
								(MaxLength >> 1) * wallDrawDirection[wallData.surface].z + //z�����Œ�l�̎��Ɏg����
								((wallData.width - (MaxLength >> 1) + 0.5f) * wallDrawDirection[wallData.surface].w));

				//�����ŖʂƂ��Ă̍��W�����Ƃ̍��W�ɖ߂�
				//�܂��ϓ�������𐳋K�ɖ߂��܂�
				wallData.setInitiPosition = wallData.createInitPosition + Float3(wallDrawDirection[wallData.surface].y,
												0.0f, wallDrawDirection[wallData.surface].w) * ((MaxLength >> 1) - 0.5f);
				//���ɕϓ����Ȃ����𐳋K�ɒ����܂�
				//�܂��ǂ���������Ɋ񂹂�
				wallData.setInitiPosition += Float3(wallDrawDirection[wallData.surface].x, 0.0f,
								wallDrawDirection[wallData.surface].z) * 0.5f;
				//���̂��ƒ��S�Ɋ񂹂邱�ƂŌ덷�𖳂���
				//-0.5f�͊��ɂ��炵�Ă���̂Ō덷������
				wallData.setInitiPosition += Float3(fabs(wallDrawDirection[wallData.surface].x), 0.0f,
								fabs(wallDrawDirection[wallData.surface].z)) * ((MaxLength >> 1) - 0.5f);
				//y�����Œ�l�ł���Ă���̂ł��炵�܂�
				wallData.setInitiPosition.y -= 0.5f;
}
void Wall::SetPushWallLength(int addLength,Float3 playerPos1,Float3 playerPos2,bool playerTurn)
{
				if(addLength > 0)
				{
								if (wallData.length >= MaxLength)
								{
												return;
								}
								Float3 chackPos = wallData.setInitiPosition - wallData.moveDirection * (wallData.length + addLength);
								if (GetBlockData((int)chackPos.x, (int)chackPos.z, (int)chackPos.y) != NON ||
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
				if (wallData.length == 4 && addLength > 0)
				{
								if (playerTurn)
								{
												wallData.pushBlockType[wallData.length - 1] = FIRST;
								}
								else
								{
												wallData.pushBlockType[wallData.length - 1] = SECOND;
								}
				}
				else if(wallData.length > 2 && addLength < 0)
				{
								if (wallData.length == 3)
								{
												wallData.PushTypeReset();
								}
								else
								{
												if (wallData.pushBlockType[wallData.length] == FIRST ||
																wallData.pushBlockType[wallData.length] == SECOND)
												{
																wallData.pushBlockType[wallData.length - 1] = wallData.pushBlockType[wallData.length];
																wallData.pushBlockType[wallData.length] = NORMAL;
												}
								}
				}
}

void Wall::ChangePushWallLine()
{
				//�����o���u���b�N�̕��т�ς���
				if (wallData.length > 3)
				{
								BlockType moveData = wallData.pushBlockType[0];
								for (int i = 0; i < wallData.length; i++)
								{
												wallData.pushBlockType[i] = wallData.pushBlockType[i + 1];
								}
								wallData.pushBlockType[wallData.length - 1] = moveData;
				}
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
																(int)wallData.setInitiPosition.y, wallData.pushBlockType[i - 1]);
								}
								wallData.moveFlag = false;
				}
}

void Wall::DownData()
{
				for (int x = 0; x < MaxLength; x++)
				{
								for (int z = 0; z < MaxLength; z++)
								{
												box[x][z].DownBlock();
								}
				}
}

void Wall::Release()
{
				ResetBlockType();
}

void Wall::Draw(bool playerMovePosDrawFlag,float downPos, float playerPosY, float nextPlayerPosY)
{
				downPos *= blockSize;
				int halfLength = MaxLength >> 1;

				//�����Ă���Œ��̃u���b�N�̕`��
				if (wallData.moveFlag)
				{
								for (int i = 0; i < (int)wallData.time + 1; i++)
								{
												block[wallData.pushBlockType[wallData.length - i - 1]].position =
																(wallData.createInitPosition + wallData.moveDirection * (i - wallData.time + 0.5f))* blockSize;
												block[wallData.pushBlockType[wallData.length - i - 1]].Draw();
								}
				}
				//�����o���ꏊ��`�悷��//����𕪂��Ă��闝�R�͎n�܂�n�_���܂������Ⴄ����
				else if (wallData.checkLengthFlag)
				{
								for (int i = 1; i <= wallData.length; i++)
								{
											 block[wallData.pushBlockType[i - 1]].position =
																(wallData.createInitPosition - wallData.moveDirection * (-0.5f + i)) * blockSize;
												block[wallData.pushBlockType[i - 1]].Draw();
								}
				}
				else if (wallData.drawTexFlag != 0)
				{
								//wallData.drawTexFlag���P�̎��̓u���b�N�������o�����Ƃ��s�\�Ȃ̂Ńe�N�X�`����ς���
								wall[wallData.drawTexFlag + 2].angles.y = 90.0f * (-wallData.surface + 1);
								wall[wallData.drawTexFlag + 2].position = wallData.createInitPosition * blockSize;
								wall[wallData.drawTexFlag + 2].Draw();
				}
			
				//���̃u���b�N�̕`��
				for (int x = -halfLength; x < halfLength; x++)
				{
								for (int z = -halfLength; z < halfLength; z++)
								{
												for (int y = 0; y < MaxHeight; y++)
												{
																//�v���C���[�̍s�����Ƃ̂ł���ꏊ��`��
																if (GetPlayerMoveFlag(x + halfLength, z + halfLength, y + 1) && playerMovePosDrawFlag)
																{
																				block[box[x + halfLength][z + halfLength].blockType[y] + 3].position = (Float3(x, y, z) + 0.5f) * blockSize;
																				block[box[x + halfLength][z + halfLength].blockType[y] + 3].Draw();
																}
																//���ɑ��݂��Ă���u���b�N�𐶐�����
																//bit���������V�t�g���Ă��̏ꏊ�̐��l��flag���f
																else if (y < MaxHeight)
																{
																				if (GetBlockData(x + halfLength, z + halfLength, y) != Wall::BlockType::NON)
																				{
																								block[box[x + halfLength][z + halfLength].blockType[y]].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[box[x + halfLength][z + halfLength].blockType[y]].position.y -= downPos;
																								block[box[x + halfLength][z + halfLength].blockType[y]].Draw();
																				}
																}
												}
												//���̕`��
												//�z����������ōl���Ă����̂ł����ŏ������ĕ`��͂�Ă�
												//�オ�v���C���[�����̏ꏊ�Ɉړ��ł��邩�̔���@�`���ς���
												if (GetPlayerMoveFlag(x + halfLength, z + halfLength, 0) && playerMovePosDrawFlag)
												{
																block[3].position = Float3(x + 0.5f, -0.5f, z + 0.5f) * blockSize;
																block[3].Draw();
												}
												else
												{
																block[0].position = Float3(x + 0.5f, -0.5f, z + 0.5f) * blockSize;
																block[0].position.y -= downPos;
																block[0].Draw();
												}
								}
				}

				wall[0].angles.x = 0.0f;
				bool pushWallType = false;
				//�O�ǂ̕`�� �܂�������܂���ƕ`��
				for (int surface = 0; surface < 4; surface++)
				{
								//�`�悷��Plane�͓����̂Ŋp�x�ύX
								//�`�悷�鏇��  �E�@���@���@��
								for (int i = 0; i < 3; i++)
								{
												wall[i].angles.y = 90.0f * (-surface + 1);
								}
								//�����O��̃}�b�v�ō쐬
								for (int y = 0; y < MaxHeight + 1; y++)
								{
												if ((playerPosY + 1 >= y || nextPlayerPosY + 1 >= y) &&
																playerPosY - 1 <= y)
												{
																pushWallType = true;
												}
												else
												{
																pushWallType = false;
												}
												for (int xz = 0; xz < halfLength; xz++)
												{
																for (int i = 0; i < 2; i++)
																{
																				if (y == MaxHeight)
																				{
																								//length + z�@�Ɓ@length + x���[ x��z��0����Ƃ���
																								wall[2].position = Float3(
																												(halfLength * wallDrawDirection[surface].x + //x�����Œ�l�̎��Ɏg����
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].y) * blockSize,//x�����ϓ�����Ƃ��Ɏg�p����
																												y * blockSize + 1.0f,
																												(halfLength * wallDrawDirection[surface].z + //z�����Œ�l�̎��Ɏg����
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].w) * blockSize);//z�����Œ�l�̎��Ɏg����
																								wall[2].Draw();
																				}
																				else
																				{
																								//length + z�@�Ɓ@length + x���[ x��z��0����Ƃ���
																								wall[pushWallType].position = Float3(
																												(halfLength * wallDrawDirection[surface].x + //x�����Œ�l�̎��Ɏg����
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].y) * blockSize,//x�����ϓ�����Ƃ��Ɏg�p����
																												y * blockSize + 1.0f,
																												(halfLength * wallDrawDirection[surface].z + //z�����Œ�l�̎��Ɏg����
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].w) * blockSize);//z�����Œ�l�̎��Ɏg����
																								wall[pushWallType].Draw();
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
