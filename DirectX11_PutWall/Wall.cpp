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

//構造体WallDataのコンストラクタ-----------------------------------------------------------
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
								//エラー報告
								return false;
				}
								return pushWallFlag[surface][x] & (1 << y);//ここにフラグ
}

void Wall::SetPushFlag(int surface, int width, int height,bool flag)
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

bool Wall::GetBlockData(int width, int depth, int height)
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

void Wall::SetBlockData(int width, int depth, int height, bool flag)
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

bool Wall::GetPlayerMoveFlag(int width, int depth, int height)
{
				if (!(width < length) || !(depth < length))
				{
								//例外処理
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
								//例外処理
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
				//対角線で判定するので+135
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
				//ここの変更処理には無駄がある可能性がある気がします
				//もう少し短縮できる気がする
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
				//GetPushFlagはそこに押し出せる場所が存在してなかったときにtrueを返す関数のため反転させる
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
				wallData.moveDirection.y = 0;//今は縦にブロックを伸ばすことが無いため0を格納している
				wallData.moveDirection.z = ((wallData.surface & 1))*(~(wallData.surface & 2) + 2);
}

//移動させるブロックの初期位置の設定
void Wall::SetInitialPosition()
{
				//ブロックを押し出す箇所を格納
				//これは倍率無いので描画で倍率をいじるように
				wallData.createInitPosition = Float3(
								((length >> 1) * wallDrawDirection[wallData.surface].x + //x軸が固定値の時に使われる
								(wallData.width - (length >> 1) + 0.5f) * wallDrawDirection[wallData.surface].y),//x軸が変動するときに使用する
								
								wallData.height + 0.5f,
								
								(length >> 1) * wallDrawDirection[wallData.surface].z + //z軸が固定値の時に使われる
								((wallData.width - (length >> 1) + 0.5f) * wallDrawDirection[wallData.surface].w));

				//ここで面としての座標をもとの座標に戻す
				//まず変動する方を正規に戻します
				wallData.setInitiPosition = wallData.createInitPosition + Float3(wallDrawDirection[wallData.surface].y,
												0.0f, wallDrawDirection[wallData.surface].w) * ((length >> 1) - 0.5f);
				//次に変動しない方を正規に直します
				//まずどちらも内側に寄せる
				wallData.setInitiPosition += Float3(wallDrawDirection[wallData.surface].x, 0.0f,
								wallDrawDirection[wallData.surface].z) * 0.5f;
				//そのあと中心に寄せることで誤差を無くす
				//-0.5fは既にずらしてあるので誤差分引く
				wallData.setInitiPosition += Float3(fabs(wallDrawDirection[wallData.surface].x), 0.0f,
								fabs(wallDrawDirection[wallData.surface].z)) * ((length >> 1) - 0.5f);
				//y軸も固定値でずれているのでずらします
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

//設定したブロックの移動
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
												//+1をする理由はinitPositionがボックスの１つ外の場所のため格納のためににずらす
												//initPositionのあり方を変えてもいい
												wallData.setInitiPosition -= wallData.moveDirection;

												SetBlockData((int)wallData.setInitiPosition.x , (int)wallData.setInitiPosition.z, 
																(int)wallData.setInitiPosition.y, true);
								}
								wallData.moveFlag = false;
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
								//物理的に座標変えます
								wall[1].position = wallData.createInitPosition * blockSize;
								wall[1].Draw();
				}
				//押し出す場所を描画する//これを分けている理由は始まる地点がまるっきり違うため
				if (wallData.checkLengthFlag)
				{
								for (int i = 1; i <= wallData.length; i++)
								{
												block[1].position = (wallData.createInitPosition - wallData.moveDirection * (-0.5f + i)) * blockSize;
												block[1].Draw();
								}
				}
				//動いている最中のブロックの描画
				else if (wallData.moveFlag)
				{
								for (int i = 0; i < wallData.length; i++)
								{
												block[0].position = (wallData.createInitPosition + wallData.moveDirection * (i - wallData.time + 0.5f))* blockSize;
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
												wall[2].position = Float3(x + 0.5f, 0, z + 0.5f) * blockSize;
												wall[2].Draw();
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
