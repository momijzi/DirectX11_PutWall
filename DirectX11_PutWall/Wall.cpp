#include"App.hpp"

Wall::Wall()
{
				App::Initialize();
				Texture wallTex(L"texture/StayBox.png");

				//事前に使用するテクスチャを貼った面を生成しておく
				wall.SetTexture(&wallTex, Float2(2.0f, 1.0f));
				wall.Create();

				//処理を軽くするために各テクスチャを貼ったボックスを用意
				block[0].SetTexture(&wallTex);
				block[0].Create(1);

				block[1].SetTexture(&wallTex);
				block[1].uvData.SetNumUvAll(Float2(1.0f, 0.0f));
				block[1].Create(1);

				block[0].scale = blockSize;
				block[1].scale = blockSize / 2;

				Release();
}

//構造体WallDataのコンストラクタ-----------------------------------------------------------
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
								//エラー報告
								return false;
				}
				return pushWallFlag[surface][x] & (1 << y);//ここにフラグ
}

void Wall::SetPushFlag(int surface, unsigned int x, unsigned int y)
{
				if (!(surface < 4) && !(y < 8))
				{
								//エラー報告
								return;
				}
				pushWallFlag[surface][x] ^= (1 << y);
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

//移動させるブロックの初期位置の設定
void Wall::SetInitialPosition(WallData &wallData)
{
				if (wallData.position == nullptr)
				{
								return;
				}
				short moveDirection[3] = {};
				//移動方向を決める
				moveDirection[0] = ((wallData.surface + 1) & 1)*(~(wallData.surface & 2) + 2);//moveDirection[0] = (((Direction + 1) % 2)*((Direction / 2) * 2 - 1));
				moveDirection[1] = 0;
				moveDirection[2] = ((wallData.surface & 1))*(~(wallData.surface & 2) + 2);
				//(((Direction & 1) + 1)*(~(Direction & 2) + 2), 0, ((Direction & 1))*(~(Direction & 2) + 2));
				for (int i = 0; i < wallData.length; i++)
				{
								//移動場所を確定させる
								wallData.position[i] = Float3(
												(float)(moveDirection[0] * (float)length + (float)(moveDirection[0]) *wallData.length*blockSize + (float)(moveDirection[0])*-i*blockSize - moveDirection[0] + !(bool)(moveDirection[0]) * (wallData.width - ((int)length >> 1) + 0.5f)*blockSize),
												(float)((wallData.height + 0.5f)*blockSize),
												(float)(moveDirection[2] * (float)length + (float)(moveDirection[2]) *wallData.length*blockSize + (float)(moveDirection[2])*-i*blockSize - moveDirection[2] + !(bool)(moveDirection[2]) * (wallData.width - ((int)length >> 1) + 0.5f)*blockSize)
								);
				}
				wallData.initPosition = wallData.position[0];

				wallData.flag = true;
}
//設定したブロックの移動
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
								//移動方向を決める
								moveDirection[0] = ((wallData.surface + 1) & 1)*(~(wallData.surface & 2) + 2);    //moveDirection[0] = (((Direction + 1) % 2)*((Direction / 2) * 2 - 1));
								moveDirection[1] = 0;
								moveDirection[2] = ((wallData.surface & 1))*(~(wallData.surface & 2) + 2);
								//(((Direction & 1) + 1)*(~(Direction & 2) + 2), 0, ((Direction & 1))*(~(Direction & 2) + 2));

								for (int i = 0; i < wallData.length; i++)
								{
												createPos[i] = wallData.initPosition;

												//移動場所を確定させる
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
																				if (GetPlayerMoveFlag(x + halfLength, z + halfLength, y))
																				{
																								block[1].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[1].Draw();
																				}
																				//中に存在しているブロックを生成する
																				//bit分だけ左シフトしてその場所の数値でflag判断
																				else if(GetBlockData(x + halfLength, z + halfLength, y))
																				{
																								block[0].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[0].Draw();
																				}
																}
												}
								}
				}
				//外壁の描画 まず周りをまるっと描画
				for (int surface = 0; surface < 4; surface++)
				{
								//描画する順番  右　下　左　上
								wall.angles.y = 90.0f * (-surface + 1);
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
																								wall.position = Float3(
																												(halfLength * wallDrawDirection[surface].x + //x軸が固定値の時に使われる
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].y) * blockSize ,//x軸が変動するときに使用する
																												y * blockSize + 1.0f,
																												(halfLength * wallDrawDirection[surface].z + //z軸が固定値の時に使われる
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].w) * blockSize);//z軸が固定値の時に使われる
																								wall.Draw();
																				}
																}
												}
								}
				}
				//最後に底面の描画
				wall.position = Float3(0.0f, -0.5f * blockSize + 1.0f, 0.0f);
				wall.angles = Float3(90.0f, 0.0f, 0.0f);
				wall.scale = Float3(length, length, 1.0f) * blockSize;
				wall.Draw();
}