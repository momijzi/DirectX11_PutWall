#pragma once

/*
壁の描画、中のブロックの描画、押し出すブロックの描画と処理を主に行う
もうすこし分割したほうがいい気もする
またblockSizeやlengthを使う位置が複数あるので
これをどうにかうまく使いたいが……
*/

class Wall
{
public:
				Wall();
				~Wall() {};
				
				//ブロックの基本サイズ
				const float blockSize = 2.0f;
				//このゲームで使用される最大のボックスの大きさ
				static const int length = 8;
				//マップの高さ　これは9以上にすると壊れるので注意
				static const int MaxHeight = 8;
				//現在のゲームクリア条件の高さ
				unsigned int height = 8;

				struct WallData
				{
								WallData() {};

								void SetWallData(int surface = 0, int width = 0, int height = 0, float time = 0.0f);
								//押し出す面、場所、長さのデータ、押し出す方向
								int surface = 0;
								int width = 0;
								int height = 0;
								int length = 1;
								Float3 moveDirection = 0.0f;
								//位置が確定したときに押し出して出てくる最初のブロックの位置を入れる
								//これはまだ位置が決定していないブロックの描画に使用する
								Float3 createInitPosition = Float3(0.0f, 0.0f, 0.0f);
								//これは押し出す位置が決まった時にデータ格納用データ
								Float3 setInitiPosition = Float3(0.0f, 0.0f, 0.0f);
								//MoveWallで主に使用 移動が完了したらfalse また描画でも使用
								bool moveFlag = false;
								float time = 0.0f;
								//このデータが現在存在しているか falseで存在している・・直そう
								//intでフラグ生成理由は、テクスチャを状況に合わせて変える必要があるため
								int drawTexFlag = 0;
								bool checkLengthFlag = false;
							
								//short moveDirection[3] = {};
				};
				WallData wallData;

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
				bool GetPushFlag(int surface, int width, int height);
				//押し出した時に指定した場所をtrueに変換する
				void SetPushFlag(int surface, int width, int height, bool flag);

				//BoxDataのblockの指定したデータを呼び出すまたは設定する
				bool GetBlockData(int width, int depth, int height);
				void SetBlockData(int width, int depth, int height, bool flag);
				
				//BoxDataのplayerMoveFlagの指定したデータを呼び出すまたは設定する
				bool GetPlayerMoveFlag(int width, int depth, int height);
				void SetPlayerMoveFlag(int width, int depth, int height,bool flag);
				//次のターンになった時に次のプレイヤーの移動場所を設定するために初期化
				void ResetPlayerMoveFlag();
				//初期の選択
				void SelectLookWall(float height, float angleY);
				//十字キーでの選択
				void SelectToWall(int moveDirection, float playerPosY, float nextPlayerPosY);
				//押し出す場所を設定したときに押し出す方向を出す
				void MoveDirectionUpdate();
				//押し出す壁の初期地点の設定
				void SetInitialPosition();
				//ブロックの押し出す量の調整に使用
				void SetPushWallLength(int addlength,Float3 playerPos1 = (-1.0f, -1.0f, -1.0f), Float3 playerPos2 = (-1.0f, -1.0f, -1.0f));
				//ブロックの押し出し時の処理
				void MoveWall();

				//初期化関数
				void Release();
				
				//ボックスの描画
				void Draw(bool playerMovePosDrawFlag, float playerPosY, float nextPlayerPosY);
				
private:
				Texture wallTex;

				//押し出すことのできる場所かどうかの判断に使用  2進数的に管理
				char pushWallFlag[4][length] = {};
				//ブロックが詰まっていく場所　ブロックがある箇所をtrueとする
				BoxData box[length][length] = {};
				//描画するデータを作成する//テクスチャ分作成（今回は処理を高速化するために）
				Plane wall[5];
				Cube block[2];

				WallData pushWallData;

				//壁の描画でその４方向で描画の仕方が変わるのでその変更用
				Float4 wallDrawDirection[4] =
				{
								Float4( 1.0f, 0.0f, 0.0f, 1.0f),//右
								Float4( 0.0f, 1.0f, 1.0f, 0.0f),//上
								Float4(-1.0f, 0.0f, 0.0f, 1.0f),//左
								Float4( 0.0f, 1.0f,-1.0f, 0.0f)	//下
				};
				/*
				//壁の描画は風車方式にした方がいいのではないか説
				//壁を選択する時に現在の方式だとかなりめんどくさい処理をしなければいけない
				//ただ修正しようにも現在の方式で進めていたため非常に変えるのが時間がかかる
				//処理としては問題ないので現在の状態で進行
				//時間ができた時に風車方式に変えていきたい
				Float4 wallDrawDirection[4] =
				{
								Float4(1.0f, 0.0f, 0.0f, 1.0f),//右
								Float4(0.0f, 1.0f, 1.0f, 0.0f),//上
								Float4(-1.0f, 0.0f, 0.0f, 1.0f),//左
								Float4(0.0f, 1.0f,-1.0f, 0.0f)	//下
				}*/
};

