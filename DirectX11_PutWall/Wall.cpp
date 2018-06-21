#include"App.hpp"

Wall::Wall()
{
				App::Initialize();
				wallTex.Load(L"texture/StayBox.png");
				wallTex.SetDivide(Float2(5.0f, 2.0f));

				//事前に使用するテクスチャを貼った面を生成しておく
				for (int i = 0; i < 5; i++)
				{
								wallTex.SetNumUvAll(Float2((float)i, 0.0f));
								wall[i].Create(&wallTex);
								wallTex.SetNumUvAll(Float2((float)i, 1.0f));
								block[i].Create(&wallTex, 1);

								wall[i].scale = block[i].scale = blockSize;
				}
				//底面の描画用に角度変更
				wall[4].angles.x = 90.0f;
				
				Release();
}

//構造体WallDataのコンストラクタ-----------------------------------------------------------
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
bool Wall::GetPushFlag(int surface, int width, int height)
{
				if (!(height < 8))
				{
								//エラー報告
								return false;
				}
				return pushWallFlag[surface][width] & (1 << height);
}

void Wall::SetPushFlag(int surface, int width, int height,bool flag)
{
				if (!(surface < 4) && !(height < MaxHeight))
				{
								//エラー報告
								return;
				}
				if ((bool)(pushWallFlag[surface][width] & (1 << height)) != flag)
				{
								pushWallFlag[surface][width] ^= (1 << height);
				}
}

Wall::BlockType Wall::GetBlockData(int width, int depth, int height)
{
				if (!(width < MaxLength) || !(depth < MaxLength) || width < 0 || depth < 0)
				{
								//例外処理
								return ERRORNUM;
				}
				if (!(height < MaxHeight))
				{
								return NON;
				}
				return box[width][depth].blockType[height];
}

void Wall::SetBlockData(int width, int depth, int height, BlockType blockType)
{
				if (!(width < MaxLength) || !(depth < MaxLength))
				{
								//例外処理
								return;
				}
				box[width][depth].blockType[height] = blockType;
}

bool Wall::GetPlayerMoveFlag(int width, int depth, int height)
{
				if (!(width < MaxLength) || !(depth < MaxLength) || width < 0 || depth < 0)
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
				if (width >= MaxLength || depth >= MaxLength || width < 0 || depth < 0)
				{
								//例外処理
								return;
				}
				box[width][depth].playerMoveBlock[height] = flag;
}

void Wall::ResetPlayerMoveFlag()
{
				for(int x = 0; x < MaxLength; x++)
				{
								for(int z = 0; z < MaxLength; z++)
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
				wallData.width = MaxLength / (int)blockSize;
				wallData.height = (int)height / (int)blockSize;
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
				else if (wallData.height > this->height - 1)
				{
								wallData.height = this->height - 1;
				}
				//押し出すことが可能な高さかどうか
				if ((playerPosY + 1 >= wallData.height || nextPlayerPosY + 1 >= wallData.height) &&
								playerPosY - 1 <= wallData.height)
				{
								if (GetPushFlag(wallData.surface, wallData.width, wallData.height) == false)
								{
												//ここにはfalseで入る予定
												wallData.drawTexFlag = 2;
												SetInitialPosition();
												return;
								}
				}
				wallData.drawTexFlag = 1;
				SetInitialPosition();
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
								((MaxLength >> 1) * wallDrawDirection[wallData.surface].x + //x軸が固定値の時に使われる
								(wallData.width - (MaxLength >> 1) + 0.5f) * wallDrawDirection[wallData.surface].y),//x軸が変動するときに使用する
								
								wallData.height + 0.5f,
								
								(MaxLength >> 1) * wallDrawDirection[wallData.surface].z + //z軸が固定値の時に使われる
								((wallData.width - (MaxLength >> 1) + 0.5f) * wallDrawDirection[wallData.surface].w));

				//ここで面としての座標をもとの座標に戻す
				//まず変動する方を正規に戻します
				wallData.setInitiPosition = wallData.createInitPosition + Float3(wallDrawDirection[wallData.surface].y,
												0.0f, wallDrawDirection[wallData.surface].w) * ((MaxLength >> 1) - 0.5f);
				//次に変動しない方を正規に直します
				//まずどちらも内側に寄せる
				wallData.setInitiPosition += Float3(wallDrawDirection[wallData.surface].x, 0.0f,
								wallDrawDirection[wallData.surface].z) * 0.5f;
				//そのあと中心に寄せることで誤差を無くす
				//-0.5fは既にずらしてあるので誤差分引く
				wallData.setInitiPosition += Float3(fabs(wallDrawDirection[wallData.surface].x), 0.0f,
								fabs(wallDrawDirection[wallData.surface].z)) * ((MaxLength >> 1) - 0.5f);
				//y軸も固定値でずれているのでずらします
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
												wallData.pushBlockType[wallData.length - 1] = SECOND;
								}
								else
								{
												wallData.pushBlockType[wallData.length - 1] = FIRST;
								}
				}
				else if(wallData.length > 2 && addLength < 0)
				{
								if (wallData.length == 3)
								{
												for (int i = 0; i < MaxLength; i++)
												{
																if (wallData.pushBlockType[i] == FIRST || wallData.pushBlockType[i] == SECOND)
																{
																				wallData.pushBlockType[i] = NORMAL;
																				break;
																}
												}
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
				//押し出すブロックの並びを変える
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
																(int)wallData.setInitiPosition.y, wallData.pushBlockType[i - 1]);
								}
								wallData.moveFlag = false;
				}
}

void Wall::Release()
{
				//falseに初期化 lengthは壁の横幅
				for (int x = 0; x < MaxLength; x++)
				{
								//壁の初期化
								for (int surface = 0; surface < 4; surface++)
								{
												pushWallFlag[surface][x] &= 0;
								}
								//中身のブロックの初期化
								for (int z = 0; z < MaxLength; z++)
								{
												box[x][z].Release();
								}
				}

				box[5][3].blockType[0] = FIRST;
				box[4][2].blockType[0] = FIRST;
				box[3][2].blockType[0] = FIRST;
				box[1][3].blockType[0] = SECOND;
				box[7][3].blockType[0] = SECOND;
				box[7][7].blockType[0] = SECOND;
				//試験的に設定している
				/*
				

				*/
}

void Wall::Draw(bool playerMovePosDrawFlag, float playerPosY, float nextPlayerPosY)
{
				int halfLength = MaxLength >> 1;

				//動いている最中のブロックの描画
				if (wallData.moveFlag)
				{
								for (int i = 0; i < (int)wallData.time + 1; i++)
								{
												block[wallData.pushBlockType[i]].position = (wallData.createInitPosition + wallData.moveDirection * (i - wallData.time + 0.5f))* blockSize;
												block[wallData.pushBlockType[i]].Draw();
								}
				}
				//押し出す場所を描画する//これを分けている理由は始まる地点がまるっきり違うため
				else if (wallData.checkLengthFlag)
				{
								for (int i = 1; i <= wallData.length; i++)
								{
											 block[wallData.pushBlockType[i - 1]].position = (wallData.createInitPosition - wallData.moveDirection * (-0.5f + i)) * blockSize;
												block[wallData.pushBlockType[i - 1]].Draw();
								}
				}
				else if (wallData.drawTexFlag != 0)
				{
								//wallData.drawTexFlagが１の時はブロックを押し出すことが不可能なのでテクスチャを変える
								wall[wallData.drawTexFlag + 1].angles.y = 90.0f * (-wallData.surface + 1);
								wall[wallData.drawTexFlag + 1].position = wallData.createInitPosition * blockSize;
								wall[wallData.drawTexFlag + 1].Draw();
				}
			
				//中のブロックの描画
				for (int x = -halfLength; x < halfLength; x++)
				{
								for (int z = -halfLength; z < halfLength; z++)
								{
												for (int y = 0; y < height + 1; y++)
												{
																for (int i = 0; i < 2; i++)
																{
																				//プレイヤーの行くことのできる場所を描画
																				if (GetPlayerMoveFlag(x + halfLength, z + halfLength, y) && playerMovePosDrawFlag)
																				{
																								block[3].position = (Float3(x, y, z) + 0.5f) * blockSize;
																								block[3].Draw();
																				}
																				//中に存在しているブロックを生成する
																				//bit分だけ左シフトしてその場所の数値でflag判断
																				else if (y < height)
																				{
																								if (GetBlockData(x + halfLength, z + halfLength, y) != Wall::BlockType::NON)
																								{
																												block[box[x + halfLength][z + halfLength].blockType[y]].position = (Float3(x, y, z) + 0.5f) * blockSize;
																												block[box[x + halfLength][z + halfLength].blockType[y]].Draw();
																								}
																				}
																}
												}
												wall[4].position = Float3(x + 0.5f, 0, z + 0.5f) * blockSize;
												wall[4].Draw();
								}
				}

				wall[0].angles.x = 0.0f;
				bool pushWallType = false;
				//外壁の描画 まず周りをまるっと描画
				for (int surface = 0; surface < 4; surface++)
				{
								//描画するPlaneは二つあるので角度変更
								//描画する順番  右　下　左　上
								for (int i = 0; i < 2; i++)
								{
												wall[i].angles.y = 90.0f * (-surface + 1);
								}
								//偶数前提のマップで作成
								for (int y = 0; y < height; y++)
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
																				if (!GetPushFlag(surface, xz + (halfLength * i), y))
																				{
																								//length + z　と　length + xが端 xとzを0からとする
																								wall[pushWallType].position = Float3(
																												(halfLength * wallDrawDirection[surface].x + //x軸が固定値の時に使われる
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].y) * blockSize,//x軸が変動するときに使用する
																												y * blockSize + 1.0f,
																												(halfLength * wallDrawDirection[surface].z + //z軸が固定値の時に使われる
																												(xz + halfLength * (i - 1) + 0.5f) * wallDrawDirection[surface].w) * blockSize);//z軸が固定値の時に使われる
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
* (float)length +//右端
wallData.length	押し出す最大の長さ																				* blockSize+
 -i *　何番目のlengthか　blockSize + 
!(bool)(wallData.moveDirection[0]) *　(wallData.width - ((int)length >> 1) + 0.5f)*blockSize),
*/
