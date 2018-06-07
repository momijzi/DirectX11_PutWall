class Wall
{
public:
				Wall()
				{
								App::Initialize();
				}
				Wall(Texture* tex)
				{
								for (int i = 0; i < MaxHeight; i++)
								{
												for (int j = 0; j < 6; j++)
												{
																tex->numUV[j] = Float2((float)i, 0.0f);
												}
												//外側法線のボックス
												mesh[i].CreateData(tex,Mesh::CUBEOUT);
								}
								for (int j = 0; j < 4; j++)
								{
												tex->numUV[j] = Float2((float)MaxHeight, 0.0f);
								}
								tex->numUV[4] = Float2((float)MaxHeight + 2, 0.0f);
								tex->numUV[5] = Float2((float)MaxHeight + 1, 0.0f);
								mesh[MaxHeight + 1].CreateData(tex, Mesh::CUBEIN);

								Release();
				}
				~Wall()
				{

				}
				//既に押し出されているかの判定
				//押し出したい箇所の検索に使うかも？
				//押し出せない場合もあるかもしれない要検討
				bool GetPushFlag(int surface, unsigned int width, unsigned int height)
				{
								if (!height < 8)
								{
												//エラー報告
												return false;
								}
									return pushWallFlag[surface][width] & (1 << height);//ここにフラグ
				}
				//押し出した時に指定した場所をtrueに変換する
				void SetPushFlag(int surface, unsigned int x, unsigned int y)
				{
								if (surface < 4 && y < 8)
								{
												//エラー報告
												return;
								}
								pushWallFlag[surface][x] ^= (1 << y);
				}

				//現在の位置
				//X移動方向.x*length+length/2+!移動方向.x*side
				//Ymap[X][Z]&(1<<i)
				//Z移動方向.z*length+length/2+!移動方向.z*side

				void Wall::PushWallPos(int direction, int side, int height, int pushLength, Float3 *pos, bool &flag)
				{
								if (pos == nullptr)
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
												pos[i] = Float3(
																(float)(moveDirection[0] * length + (bool)(moveDirection[0]) * (length >> 1) + (bool)(moveDirection[0])*-i + !(bool)(moveDirection[0]) * side),
																(float)(height),
																(float)(moveDirection[2] * (float)length + (bool)(moveDirection[2]) * (length >> 1) + (bool)(moveDirection[2])*-i + !(bool)(moveDirection[2]) * side)
												);
								}

								flag = true;
				}

				//指定した箇所のデータを渡す　現在そこにブロックが存在しているかの判断
				bool GetWallData(unsigned int width, unsigned int depth, unsigned int height)
				{
								if (width < length || depth < length)
								{
												//例外処理
												return true;
								}
								return map[width][depth] & (1 << height);
				}
				//指定した箇所にデータを入れる　そこにブロックを追加する
				void Wall::SetWallData(unsigned int width, unsigned int depth, unsigned int height, bool flag)
				{
								if (width < length || depth < length)
								{
												//例外処理
								}

								if ((bool)(map[width][depth] & (1 << height)) != flag)
								{
												map[width][depth] ^= (1 << height);
								}
				}
				//ブロックの押し出し時の処理
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

				//初期化
				void Release()
				{
								//falseに初期化
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
				//ボックスの描画
				void Draw()
				{
								for (int x = 0; x < length; x++)
								{
												for (int z = 0; z < length; z++)
												{
																for (int y = 0; y < height; y++)
																{
																				//bit分だけ左シフトしてその場所の数値でflag判断
																				if (map[x][z] & (1 << y))
																				{
																								mesh[y].Draw(Float3((float)x,(float)y,(float)z));
																				}
																}
												}
								}
								mesh[MaxHeight + 1].Draw();
				}
private:

				//このゲームで使用される最大のボックスの大きさ
				static const unsigned int length = 10;
				//マップの高さ　これは9以上にすると壊れるので注意
				static const unsigned int MaxHeight = 8;
				unsigned int height = 8;

				//押し出すことのできる場所かどうかの判断に使用  2進数的に管理
				char pushWallFlag[4][length] = {};
				//押し出されたブロックを格納する　描画と当たり判定に使用
				char map[length][length] = {};

				//描画するボックスのデータを作成する
				Mesh mesh[MaxHeight + 3] = {};
};