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
												box[x][z].ResetBlock();
								}
				}
}
void Wall::ResetPlayerMoveFlag()
{
				for (int x = 0; x < MaxLength; x++)
				{
								for (int z = 0; z < MaxLength; z++)
								{
												for (int y = 0; y <= MaxHeight; y++)
												{
																box[x][z].playerMoveBlock[y] = false;
												}
								}
				}
}

Wall::BlockType Wall::GetBlockData(Float3 pos)
{
				if (!(pos.x < MaxLength) || !(pos.z < MaxLength) || pos.x < 0 || pos.z < 0 )
				{
								//��O����
								return ERRORNUM;
				}
				return box[(int)pos.x][(int)pos.z].blockType[(int)pos.y];
}

void Wall::SetBlockData(Float3 pos, BlockType blockType)
{
				if (!(pos.x < MaxLength) || !(pos.z < MaxLength) || pos.x < 0 || pos.z < 0 || pos.y > this->MaxHeight)
				{
								//��O����
								return;
				}
				box[(int)pos.x][(int)pos.z].blockType[(int)pos.y] = blockType;
}

bool Wall::GetPlayerMoveFlag(Float3 pos)
{
				if (!(pos.x < MaxLength) || !(pos.z < MaxLength) || pos.x < 0 || pos.z < 0)
				{
								//��O����
								return false;
				}
				return box[(int)pos.x][(int)pos.z].playerMoveBlock[(int)pos.y];
}
void Wall::SetPlayerMoveFlag(Float3 pos)
{
				if (!(pos.x < MaxLength) || !(pos.z < MaxLength) || pos.x < 0 || pos.z < 0)
				{
								//��O����
								return;
				}
				box[(int)pos.x][(int)pos.z].playerMoveBlock[(int)pos.y] = true;
}

void Wall::SelectLookWall(float playerHeight, float angleY)
{
				//�Ίp���Ŕ��肷��̂�+135
				if (angleY < 0)
				{
								angleY += ((-(int)angleY / 360) + 1) * 360;
				}
				wallData.surface = (360 - ((int)angleY + 225) % 360) / 90;
				wallData.width = MaxLength / (int)blockSize - 1;
				wallData.height = (int)playerHeight;
				MoveDirectionUpdate();
}

//y���̍��W�����Ȃ��ꏊ�ɍs�����Ƃ����Ƃ�
//�����Ȃ��悤��SE�𗬂��̂�bool��Ԃ��Ĕ��f
bool Wall::SelectToWall(int moveDirection, Float2 bothPlayerPosY)
{
				wallData.height -= ((moveDirection & 2)*(moveDirection & 1)) - (moveDirection & 1);
				wallData.width -= ((((moveDirection - 1) & 2)*((moveDirection - 1) & 1)) - ((moveDirection - 1) & 1)) *
								-(((((wallData.surface + 1) & 3) >> 1) << 1) - 1);

				if (wallData.width < 0)
				{
								wallData.width = (!(bool)(wallData.surface & 2))*(MaxLength - 1);
								wallData.surface += ((((wallData.surface + 1) & 3) >> 1) << 1) - 1;
								MoveDirectionUpdate();
				}
				else if (wallData.width >= MaxLength)
				{
								wallData.width = (!(bool)(wallData.surface & 2))*(MaxLength - 1);
								wallData.surface -= ((((wallData.surface + 1) & 3) >> 1) << 1) - 1;
								MoveDirectionUpdate();
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
								return false;
				}
				//-2�̗��R�@����0�Ԓn�ɑ��݂��Ă���̂łP�オ���Ă��邽��-1
				//�S�[���̍����͉����o���Ȃ��悤�ɂ���̂�MaxHeight�̍����͉����o���Ȃ��̂�-1 �C�R�[��-2
				//�G���E�E�������ԂȂ��̂ň�U�ۗ��A���ƂŐ��K�ɒ����ׂ�
				else if (wallData.height > this->MaxHeight - 2)
				{
								wallData.height = this->MaxHeight - 2;
								return false;
				}
				SetInitialPosition();
				//�����o�����Ƃ��\�ȍ������ǂ���
				if ((bothPlayerPosY.x + 1 > wallData.height || bothPlayerPosY.y + 1 > wallData.height) &&
								bothPlayerPosY.x - 2 <= wallData.height)
				{
								if (GetBlockData(wallData.setInitiPosition - wallData.moveDirection) == NON)
								{
												//�����o����Ƀf�[�^�����݂��邩
												wallData.drawTexFlag = 2;
												return true;
								}
				}
				wallData.drawTexFlag = 1;
				return true;
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
								
								wallData.height + 1.5f,
								
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
								if (GetBlockData(chackPos) != NON ||
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
				wallData.time += App::GetDeltaTime() * wallData.length;
				
				if (wallData.time > wallData.length)
				{
								wallData.moveFlag = false;

								for (int i = 1; i <= wallData.length; i++)
								{
												//+1�����闝�R��initPosition���{�b�N�X�̂P�O�̏ꏊ�̂��ߊi�[�̂��߂ɂɂ��炷
												//initPosition�̂������ς��Ă�����
												wallData.setInitiPosition -= wallData.moveDirection;

												SetBlockData(wallData.setInitiPosition, wallData.pushBlockType[i - 1]);
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

void Wall::Draw(bool playerMovePosDrawFlag,float downPos, Float2 bothPlayerPosY)
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
												//���B�n�_�̍���+ �����K�v�Ȃ̂ŏ���+1
												for (int y = 0; y <= MaxHeight; y++)
												{
																//�v���C���[�̍s�����Ƃ̂ł���ꏊ��`��
																if (GetPlayerMoveFlag(Float3(x + halfLength, y + 1, z + halfLength)) && playerMovePosDrawFlag)
																{
																				block[box[x + halfLength][z + halfLength].blockType[y] + 3].position = (Float3(x, y, z) + 0.5f) * blockSize;
																				block[box[x + halfLength][z + halfLength].blockType[y] + 3].Draw();
																}
																//���ɑ��݂��Ă���u���b�N�𐶐�����
																//bit���������V�t�g���Ă��̏ꏊ�̐��l��flag���f
																else
																{
																				if (GetBlockData(Float3(x + halfLength,y, z + halfLength)) != Wall::BlockType::NON)
																				{
																								block[box[x + halfLength][z + halfLength].blockType[y]].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[box[x + halfLength][z + halfLength].blockType[y]].position.y -= downPos;
																								block[box[x + halfLength][z + halfLength].blockType[y]].Draw();
																				}
																}
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
								//���B�n�_�̍���+ �����K�v�Ȃ̂ŏ���+1
								//�S�[���ƂȂ鍂���ɕ`������Ȃ��Ƃ����Ȃ�����
								for (int y = 0; y <= MaxHeight; y++)
								{
												if ((bothPlayerPosY.x + 1 >= y || bothPlayerPosY.y + 1 >= y) &&
																bothPlayerPosY.x - 1 <= y)
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
