#include"App.hpp"

Wall::Wall()
{
				App::Initialize();
				wallTex.Load(L"texture/StayBox.png");
				wallTex.SetDivide(Float2(4.0f, 1.0f));

				//事前に使用するテクスチャを貼った面を生成しておく
				wall[0].Create(&wallTex);

				//処理を軽くするために各テクスチャを貼ったボックスを用意
				block[0].Create(&wallTex,1);

				wallTex.SetNumUvAll(Float2(1.0f, 0.0f));
				block[1].Create(&wallTex,1);

				wallTex.SetNumUvAll(Float2(2.0f, 0.0f));
				wall[1].Create(&wallTex);


				wall[1].scale = blockSize;

				block[0].scale = blockSize;
				block[1].scale = blockSize;

				Release();
}

//構造体WallDataのコンストラクタ-----------------------------------------------------------
void Wall::WallData::SetWallData(int surface, int width, int height, float time)
{
				this->surface = surface;
				this->width = width;
				this->height = height;
				position = new Float3[length];
				for (int i = 0; i < length; i++)
				{
								position[i] = {};
				}
				moveFlag = false;
				checkLengthFlag = false;
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
								//エラー報告
								return false;
				}
		
								return pushWallFlag[surface][x] & (1 << y);//ここにフラグ
}

void Wall::SetPushFlag(int surface, unsigned int width, unsigned int height,bool flag)
{
				if (!(surface < 4) && !(height < 8))
				{
								//エラー報告
								return;
				}
				if ((bool)(pushWallFlag[surface][width] & (1 << height)) != flag)
				{
								pushWallFlag[surface][width] ^= (1 << height);
				}
}

bool Wall::GetBlockData(unsigned int width, unsigned int depth, unsigned int height)
{
				if (!(width < length) || !(depth < length))
				{
								//例外処理
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
								//例外処理
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
								//例外処理
								return false;
				}
				return box[width][depth].playerMoveBlock[height];
}
void Wall::SetPlayerMoveFlag(unsigned int width, unsigned int depth, unsigned int height,bool flag)
{
				if (!(width < length) || !(depth < length))
				{
								//例外処理
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
				//対角線で判定するので+135
				wallData.surface = (((int)angleY % 360) + 135) / 90;
				wallData.width = length / (int)blockSize;
				wallData.height = (int)height / (int)blockSize;
}

void Wall::SelectToWall(int moveDirection)
{
				if (moveDirection != 0)
				{
								SetPushFlag(wallData.surface, wallData.width, wallData.height, false);

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
				wallData.drawTexFlag = GetPushFlag(wallData.surface, wallData.width, wallData.height);
				if (wallData.drawTexFlag != true)
				{
								SetPushFlag(wallData.surface, wallData.width, wallData.height,true);
				}
}


//移動させるブロックの初期位置の設定
void Wall::SetInitialPosition()
{
				wallData.drawTexFlag = true;
				if (wallData.position == nullptr)
				{
								return;
				}
				MoveDirectionUpdate();
				//(((Direction & 1) + 1)*(~(Direction & 2) + 2), 0, ((Direction & 1))*(~(Direction & 2) + 2));
				for (int i = 0; i < wallData.length; i++)
				{
								//移動場所を確定させる
								wallData.position[i] = Float3(
												(((length >> 1) + (wallData.length - 1) - i + 0.5f)* wallData.moveDirection[0] +
												(wallData.width - ((int)length >> 1) + 0.5f) * !(bool)(wallData.moveDirection[0]))* blockSize,

												(float)((wallData.height + 0.5f) * blockSize * !wallData.moveDirection[1]),
												
												(((length >> 1) + (wallData.length - 1) - i + 0.5f) * wallData.moveDirection[2] +
												(wallData.width - ((int)length >> 1) + 0.5f) * !(bool)wallData.moveDirection[2])* blockSize
								);
				}
				wallData.initPosition = wallData.position[wallData.length - 1];

				wallData.checkLengthFlag = false;
				wallData.moveFlag = true;
}
//設定したブロックの移動
void Wall::MoveWall()
{
				if (!wallData.moveFlag)
				{
								return;
				}
				for (int i = 0; i < wallData.length; i++)
				{
								wallData.position[i] -= Float3(
												(wallData.moveDirection[0]),
												0,
												(wallData.moveDirection[2])
								)*App::GetDeltaTime()*blockSize;
				}
				wallData.time += App::GetDeltaTime();

				if (wallData.time > wallData.length)
				{
								wallData.moveFlag = false;
								Float3 *createPos;
								createPos = new Float3[wallData.length];
								//(((Direction & 1) + 1)*(~(Direction & 2) + 2), 0, ((Direction & 1))*(~(Direction & 2) + 2));

								for (int i = 0; i < wallData.length; i++)
								{
												createPos[i] = wallData.initPosition;

												//移動場所を確定させる
												createPos[i] = Float3((int)createPos[i].x >> 1, (int)createPos[i].y >> 1, (int)createPos[i].z >> 1);

												createPos[i] -= -Float3(wallData.moveDirection[0], wallData.moveDirection[1], wallData.moveDirection[2])*
																(((int)length - (wallData.length + 1)) >> 1) + Float3(wallData.moveDirection[0],
																wallData.moveDirection[1],
																wallData.moveDirection[2])*(i + (wallData.length >> 1));
												
												createPos[i] = Float3(((int)length >> 1) * !(bool)wallData.moveDirection[0] +
																createPos[i].x + ((int)wallData.surface >> 1) * (int)length*(bool)wallData.moveDirection[0],
															
																createPos[i].y, 
																
																((int)length >> 1) * !(bool)wallData.moveDirection[2] + createPos[i].z +
																((int)wallData.surface >> 1) * (int)length*(bool)wallData.moveDirection[2]);

												//createPos[i] += Float3(length*moveDirection[0], 0, length*moveDirection[2]);
												SetBlockData((unsigned int)createPos[i].x, (unsigned int)createPos[i].z, (unsigned int)createPos[i].y, true);
								}
								delete[] createPos;
				}
}

void Wall::MoveDirectionUpdate()
{
				wallData.moveDirection[0] = ((wallData.surface + 1) & 1)*(~(wallData.surface & 2) + 2);//moveDirection[0] = (((Direction + 1) % 2)*((Direction / 2) * 2 - 1));
				wallData.moveDirection[1] = 0;
				wallData.moveDirection[2] = ((wallData.surface & 1))*(~(wallData.surface & 2) + 2);
}

Float3 Wall::GetWallSelectPosition()
{
				MoveDirectionUpdate();
				return Float3(
								(float)(wallData.moveDirection[0] * (float)length + (float)(wallData.moveDirection[0]) *wallData.length*blockSize + !(bool)(wallData.moveDirection[0]) * (wallData.width - ((int)length >> 1) + 0.5f)*blockSize),
								(float)((wallData.height + 0.5f)*blockSize),
								(float)(wallData.moveDirection[2] * (float)length + (float)(wallData.moveDirection[2]) *wallData.length*blockSize + !(bool)(wallData.moveDirection[2]) * (wallData.width - ((int)length >> 1) + 0.5f)*blockSize)
				);
}

void Wall::Release()
{
				//falseに初期化 lengthは壁の横幅
				for (int x = 0; x < length; x++)
				{
								//壁の初期化
								for (int surface = 0; surface < 4; surface++)
								{
												pushWallFlag[surface][x] &= 0;
								}
								//中身のブロックの初期化
								for (int z = 0; z < length; z++)
								{
												box[x][z].Release();
								}
				}
				//試験的に設定している
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

				wall[0].scale = blockSize;
				wall[0].angles.x = 90.0f;

				if (!wallData.drawTexFlag)
				{
								wall[1].angles.y = 90.0f * (-wallData.surface + 1);
								//物理的に座標変えます
								wall[1].position = GetWallSelectPosition();
								wall[1].Draw();
				}

				//押し出す場所を描画する
				if (wallData.checkLengthFlag)
				{
								for (int i = 0; i < wallData.length; i++)
								{
												block[0].position = GetWallSelectPosition() /*- Float3((float)(wallData.moveDirection[0])* i *blockSize, 0, (float)(wallData.moveDirection[2])* i *blockSize)*/;
												block[0].Draw();
								}
				}
				//移動しているときのブロックを描画する
				if (wallData.moveFlag)
				{
								for (int i = 0; i < wallData.length; i++)
								{
												block[0].position = wallData.position[i];
												block[0].Draw();
								}
				}

				//中のブロックの描画
				for (int x = -halfLength; x < halfLength; x++)
				{
								for (int z = -halfLength; z < halfLength; z++)
								{
												for (int y = 0; y < height; y++)
												{
																for (int i = 0; i < 2; i++)
																{
																				//プレイヤーの行くことのできる場所を描画
																				if (GetPlayerMoveFlag(x + halfLength, z + halfLength, y) && playerMovePosDrawFlag)
																				{
																								block[1].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[1].Draw();
																				}
																				//中に存在しているブロックを生成する
																				//bit分だけ左シフトしてその場所の数値でflag判断
																				else if (GetBlockData(x + halfLength, z + halfLength, y))
																				{
																								block[0].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[0].Draw();
																				}
																}
												}
												wall[0].position = Float3(x + 0.5f, 0, z + 0.5f) * blockSize;
												wall[0].Draw();
								}
				}

				wall[0].angles.x = 0.0f;

				//外壁の描画 まず周りをまるっと描画
				for (int surface = 0; surface < 4; surface++)
				{
								//描画する順番  右　下　左　上
								wall[0].angles.y = 90.0f * (-surface + 1);
								//偶数前提のマップで作成
								for (int xz = 0; xz < halfLength; xz++)
								{
												for (int y = 0; y < height; y++)
												{
																for (int i = 0; i < 2; i++)
																{
																				if (!GetPushFlag(surface, xz + (halfLength * i), y))
																				{
																								//length + z　と　length + xが端 xとzを0からとする
																								wall[0].position = Float3(
																												(halfLength * wallDrawDirection[surface].x + //x軸が固定値の時に使われる
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].y) * blockSize ,//x軸が変動するときに使用する
																												y * blockSize + 1.0f,
																												(halfLength * wallDrawDirection[surface].z + //z軸が固定値の時に使われる
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].w) * blockSize);//z軸が固定値の時に使われる
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
* (float)length +//右端
wallData.length	押し出す最大の長さ																				* blockSize+
 -i *　何番目のlengthか　blockSize + 
!(bool)(wallData.moveDirection[0]) *　(wallData.width - ((int)length >> 1) + 0.5f)*blockSize),
*/
