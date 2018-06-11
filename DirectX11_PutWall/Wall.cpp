#include"App.hpp"

Wall::Wall(Texture* tex)
{
				//事前に使用するテクスチャを貼った面を生成しておく
				wall.Create(tex);
				block.Create(tex,1);

				Release();
}

bool Wall::GetPushFlag(int surface, unsigned int width, unsigned int height)
{
				if (!height < 8)
				{
								//エラー報告
								return false;
				}
				return pushWallFlag[surface][width] & (1 << height);//ここにフラグ
}

void Wall::SetPushFlag(int surface, unsigned int x, unsigned int y)
{
				if (surface < 4 && y < 8)
				{
								//エラー報告
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
								//移動方向を決める
								moveDirection[0] = ((direction + 1) & 1)*(~(direction & 2) + 2);    //moveDirection[0] = (((Direction + 1) % 2)*((Direction / 2) * 2 - 1));
								moveDirection[1] = 0;
								moveDirection[2] = ((direction & 1))*(~(direction & 2) + 2);
								//(((Direction & 1) + 1)*(~(Direction & 2) + 2), 0, ((Direction & 1))*(~(Direction & 2) + 2));

								//移動場所を確定させる
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
								//例外処理
								return true;
				}
				return box[width][depth] & (1 << height);
}

void Wall::SetWallData(unsigned int width, unsigned int depth, unsigned int height, bool flag)
{
				if (width < length || depth < length)
				{
								//例外処理
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
				//出来ればタイムが取得したかったでござる
				for (int i = 0; i < length; i++)
				{
								pos[i] -= Float3(
												((direction + 1) & 1)*(~(direction & 2) + 2),
												0,
												((direction & 1))*(~(direction & 2) + 2)
								)*((float)1 / 6);//本来はdeltaTimeを使いたいところ
				}

				if ((float)timeCount / 6 > length)//ココのif文がおかしいので後で修正
				{
								flag = false;
								//ココで無理やり指定位置にそろえるべきだと思う
				}
}

void Wall::Release()
{
				//falseに初期化 lengthは壁の横幅
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
				//												//bit分だけ左シフトしてその場所の数値でflag判断
				//												if (box[x][z] & (1 << y))
				//												{
				//																wall[y].position = Float3((float)x, (float)y, (float)z);
				//																wall[y].Draw();
				//												}
				//								}
				//				}
				//}
				//外壁の描画 まず周りをまるっと描画
				for (int i = 0; i < 4; i++)
				{
								//描画する順番  右　下　左　上
								wall.angles.y = 90.0f * (i + 1);
								//偶数前提のマップで作成
								for (int xz = 0; xz < halfLength; xz++)
								{
												for (int y = 0; y < height; y++)
												{
																for (int sign = -1; sign < 2; sign += 2)
																{
																				if (!GetPushFlag(i, halfLength + (xz * sign), y))
																				{
																								//length + z　と　length + xが端 xとzを0からとする
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
				//最後に底面の描画
				wall.position = Float3(0.0f, -0.5f * blockSize + 1.0f, 0.0f);
				wall.angles = Float3(90.0f,0.0f,0.0f);
				wall.scale = Float3(length, length, 1.0f) * blockSize;
				wall.Draw();
}