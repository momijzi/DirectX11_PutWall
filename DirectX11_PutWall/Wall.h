#pragma once
class Wall
{
public:
				Wall()
				{
								App::Initialize();
				}
				Wall(Texture* tex);
				
				~Wall()
				{

				}

				struct WallData
				{
								WallData(int surface, int width, int height, int length, float time);
								
								~WallData();
								
								int surface = 0;
								int width = 0;
								int height = 0;
								int length = 0;
								Float3 *position;
								Float3 initPosition = Float3(0, 0, 0);
								bool flag = false;
								float time = 0.0f;
				};

				/*
				struct WallData2
				{
								Float3 position;//押し出すブロックの先頭座標
								int length;//押し出す長さ
								int pushDirection;//押し出す方向
								bool activeFlag;//現在押し出す処理をしているか
								Float2 Direction[4] = {
												Float2(-1.0f, 0.0f),//左
												Float2(0.0f, 1.0f), //前
												Float2(1.0f, 0.0f), //右
												Float2(0.0f, -1.0f)	//後
								};
				};
				void CreateBlock();
				*/

				//4方面の壁から現在ブロックが出ているかの判断に使用する
				bool GetPushFlag(int surface, unsigned int x, unsigned int y);
				//押し出した時に指定した場所をtrueに変換する
				void SetPushFlag(int surface, unsigned int x, unsigned int y);
				//指定した箇所のデータを渡す　現在そこにブロックが存在しているかの判断
				bool GetBoxData(unsigned int width, unsigned int depth, unsigned int height);
				//指定した箇所にデータを入れる　そこにブロックを追加する 主に押し出したブロックが静止したときにその場所にtrueを入れまくる
				void SetBoxData(unsigned int width, unsigned int depth, unsigned int height, bool flag);
				
				//押し出す壁の初期地点の設定
				void SetInitialPosition(WallData &wallData);
				//ブロックの押し出し時の処理
				void MoveWall(WallData &wallData);

				//初期化関数
				void Release();
				
				//ボックスの描画
				void Draw();
				
private:
				//ブロックの基本サイズ
				const float blockSize = 2.0f;
				//このゲームで使用される最大のボックスの大きさ
				static const unsigned int length = 8;
				//マップの高さ　これは9以上にすると壊れるので注意
				static const unsigned int MaxHeight = 8;
				unsigned int height = 5;
				//押し出すことのできる場所かどうかの判断に使用  2進数的に管理
				char pushWallFlag[4][length] = {};
				//ブロックが詰まっていく場所　ブロックがある箇所をtrueとする
				char box[length][length] = {};
				//描画するデータを作成する//テクスチャ分作成（今回は処理を高速化するために）
				Plane wall;
				Cube block;
				//壁の描画でその４方向で描画の仕方が変わるのでその変更用
				Float4 wallDrawDir[4] =
				{
								Float4(1.0f,0.0f,0.0f,1.0f),				//右
								Float4(0.0f,1.0f,1.0f,0.0f),					//上
								Float4(-1.0f,0.0f,0.0f,1.0f),			//左
								Float4(0.0f,1.0f,-1.0f,0.0f)			//下
				};
};

