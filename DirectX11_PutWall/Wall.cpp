#include"App.hpp"

Wall::Wall(Texture* tex)
{
				//事前に使用するテクスチャを貼った面を生成しておく
				wall.Create(tex);
				block.Create(tex, 1);
				block.scale = blockSize;

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
bool Wall::GetPushFlag(int surface, unsigned int width, unsigned int height)
{
				if (!(height < 8))
				{
								//エラー報告
								return false;
				}
				return pushWallFlag[surface][width] & (1 << height);//ここにフラグ
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

bool Wall::GetWallData(unsigned int width, unsigned int depth, unsigned int height)
{
				if (!(width < length) || !(depth < length))
				{
								//例外処理
								return true;
				}
				return box[width][depth] & (1 << height);
}

void Wall::SetWallData(unsigned int width, unsigned int depth, unsigned int height, bool flag)
{
				if (!(width < length) || !(depth < length))
				{
								//例外処理
				}

				if ((bool)(box[width][depth] & (1 << height)) != flag)
				{
								box[width][depth] ^= (1 << height);
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
												(float)(moveDirection[0] * (float)length + (float)(moveDirection[0]) *wallData.length*blockSize + (float)(moveDirection[0])*-i*blockSize - moveDirection[0] + !(bool)(moveDirection[0]) * wallData.width),
												(float)((wallData.height + 0.5f)*blockSize),
												(float)(moveDirection[2] * (float)length + (float)(moveDirection[2]) *wallData.length*blockSize + (float)(moveDirection[2])*-i*blockSize - moveDirection[2] + !(bool)(moveDirection[2]) * wallData.width)
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
				//if (wallData.flag)
				//{
				//出来ればタイムが取得したかったでござる
				for (int i = 0; i < wallData.length; i++)
				{
								wallData.position[i] -= Float3(
												((wallData.surface + 1) & 1)*(~(wallData.surface & 2) + 2),
												0,
												((wallData.surface & 1))*(~(wallData.surface & 2) + 2)
								)*App::GetDeltaTime()*blockSize;//本来はdeltaTimeを使いたいところ
				}
				//}
				for (int i = 0; i < wallData.length; i++)
				{
								block.scale = blockSize;
								block.position = wallData.position[i];
								block.Draw();
				}
				if (wallData.time > wallData.length)//ココのif文がおかしいので後で修正
				{
								wallData.flag = false;

								//SetWallData(wallData.width, wallData.depth, unsigned int height, bool flag)
								//ココで無理やり指定位置にそろえるべきだと思う

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
												//移動場所を確定させる
												createPos[i] = wallData.initPosition;

												createPos[i] -= Float3(moveDirection[0], 0, moveDirection[2])*length*blockSize - Float3((bool)moveDirection[0], 0, (bool)moveDirection[2]);

												createPos[i].y = (int)createPos[i].y >> 1;

												//createPos[i] += Float3(length*moveDirection[0], 0, length*moveDirection[2]);
												SetWallData((unsigned int)createPos[i].x, (unsigned int)createPos[i].z, (unsigned int)createPos[i].y, true);
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
												box[x][z] &= 0;
								}
				}
				/*
				pushWallFlag[3][2] ^= (1 << 1);
				pushWallFlag[3][4] ^= (1 << 2);
				pushWallFlag[3][1] ^= (1 << 4);
				pushWallFlag[3][5] ^= (1 << 3);
				pushWallFlag[3][6] ^= (1 << 0);
				*/
				/*
				box[2][1] ^= (1 << 2);
				box[1][3] ^= (1 << 3);
				box[3][1] ^= (1 << 2);
				box[5][1] ^= (1 << 2);
				box[2][6] ^= (1 << 2);
				*/
}

void Wall::Draw()
{
				int halfLength = length >> 1;

				block.scale = blockSize;
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
																				//bit分だけ左シフトしてその場所の数値でflag判断
																				if (GetWallData(x + halfLength, z + halfLength, y))
																				{
																								block.position = (Float3(x,y,z) + 0.5f) * blockSize;
																								block.Draw();
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
																												(halfLength * wallDrawDir[surface].x + //x軸が固定値の時に使われる
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDir[surface].y) * blockSize ,//x軸が変動するときに使用する
																												y * blockSize + 1.0f,
																												(halfLength * wallDrawDir[surface].z + //z軸が固定値の時に使われる
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDir[surface].w) * blockSize);//z軸が固定値の時に使われる
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