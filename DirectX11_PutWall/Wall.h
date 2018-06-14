#pragma once

/*
壁の描画、中のブロックの描画、押し出すブロックの描画と処理を主に行う
もうすこし分割したほうがいい気もする
*/

class Wall
{
public:
				Wall();
				~Wall() {};
private:
				//ブロックの基本サイズ
				const float blockSize = 2.0f;
				//このゲームで使用される最大のボックスの大きさ
				static const unsigned int length = 8;
				//マップの高さ　これは9以上にすると壊れるので注意
				static const unsigned int MaxHeight = 8;
				//現在のゲームクリア条件の高さ
				unsigned int height = 5;
public:
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

				struct BoxData
				{
								//ブロックが存在しているか
								char block;
								//プレイヤーが移動可能な場所// +1の理由は8段の高さがあってその上がクリアの高さなため
								bool playerMoveBlock[MaxHeight + 1];

								void Release()
								{
												block &= 0;
												for (int i = 0; i < MaxHeight + 1; i++)
												{
																playerMoveBlock[i] = false;
												}
								}
				};

				//4方面の壁から現在ブロックが出ているかの判断に使用する
				bool GetPushFlag(int surface, unsigned int x, unsigned int y);
				//押し出した時に指定した場所をtrueに変換する
				void SetPushFlag(int surface, unsigned int x, unsigned int y);

				//BoxDataのblockの指定したデータを呼び出すまたは設定する
				bool GetBlockData(unsigned int width, unsigned int depth, unsigned int height);
				void SetBlockData(unsigned int width, unsigned int depth, unsigned int height, bool flag);
				
				//BoxDataのplayerMoveFlagの指定したデータを呼び出すまたは設定する
				bool GetPlayerMoveFlag(unsigned int width, unsigned int depth, unsigned int height);
				void SetPlayerMoveFlag(unsigned int width, unsigned int depth, unsigned int height,bool flag);
				
				//次のターンになった時に次のプレイヤーの移動場所を設定するために初期化
				void ResetPlayerMoveFlag();

				//押し出す壁の初期地点の設定
				void SetInitialPosition(WallData &wallData);
				//ブロックの押し出し時の処理
				void MoveWall(WallData &wallData);

				//初期化関数
				void Release();
				
				//ボックスの描画
				void Draw();
				
private:
				//押し出すことのできる場所かどうかの判断に使用  2進数的に管理
				char pushWallFlag[4][length] = {};
				//ブロックが詰まっていく場所　ブロックがある箇所をtrueとする
				BoxData box[length][length] = {};
				//描画するデータを作成する//テクスチャ分作成（今回は処理を高速化するために）
				Plane wall;
				Cube block[2];
				//壁の描画でその４方向で描画の仕方が変わるのでその変更用
				Float4 wallDrawDirection[4] =
				{
								Float4( 1.0f, 0.0f, 0.0f, 1.0f),//右
								Float4( 0.0f, 1.0f, 1.0f, 0.0f),//上
								Float4(-1.0f, 0.0f, 0.0f, 1.0f),//左
								Float4( 0.0f, 1.0f,-1.0f, 0.0f)	//下
				};
};

