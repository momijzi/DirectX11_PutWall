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
												return;
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

				bool GetWallData(unsigned int width, unsigned int depth, unsigned int height)
				{
								if (!height < 8)
								{
												//最上部なためブロックは存在しない
												return false;
								}
								return map[width][depth] & (1 << height);
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