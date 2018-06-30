#pragma once

class PlayerManager
{
public:
				PlayerManager();
				~PlayerManager() {};

				//プレイヤーが現在移動場所を選択している状態かをセットする
				void SetDrawFlag(bool flag) { player[turn].moveFlag = flag; }
				//上の取得バージョン
				bool GetDrawFlag() { return player[turn].moveFlag; }
				//現在ターンが来ているプレイヤーの押し出せる量
				int GetCurrentTurnPushLength() { return player[turn].length; }
				//壁を押し出したので押し出した分だけ引く
				void SetPushLength(int length) { player[turn].length -= length; }
				//引数に現在のステージの大きさを入れておく
				//これでステージの大きさを最大保持量として保管する
				void PlusPushLength(int wallLength);
			
				//turnが来ているプレイヤーの座標か来てない座標か指定して渡す
				Float3 GetPosition(bool flag);
				
				//現在ターンがきているプレイヤーの取得
				bool GetTurn() { return turn; }

				//UIを描画するために使用
				Float2 GetPushLength() { return Float2((float)player[0].length, (float)player[1].length); }

				//プレイヤーがこのターン移動できる場所を検索
				void MovementRange(Wall* wall, Float3 movePos = Float3(0.0f, 0.0f, 0.0f),
								int Direction = -1, int moveCount = 1);

				//移動が完了していたら
				void MoveableChack(Wall* wall, int Direction);
				
				//一マス以上の移動をしているか
				bool MoveFlagChack();

				//ステージが下降した時
				bool DownPlayer();

				//次のプレイヤーのターンにする
				void NextTurn()
				{
								player[turn].movePosition = 0.0f;
								turn = !turn;
				}

				//プレイヤーの描画
				void Draw(int boxLength, int blockSize,float downPos);

				//リプレイ時に初期地点に戻すために使う
				void Release(float mapLength);

private:
				struct Player
				{
								Player();
								//描画用の座標
								Float3 position;
								//仮の移動する座標を描画するのに使用（判別にも使用）
								Float3 movePosition;
								//移動を決めている最中かどうかのフラグ
								//これで仮のプレイヤーを描画するかを決める
								//ただしAlphaを半透明にしたいができないのでなんとも・・
								bool moveFlag;
								//押し出せるブロックの量
								int length;
				};
				Player player[2];
				//プレイヤーが一ターンに動くことのできるマス数
				const int MaxMove = 2;
				const int plusPoint = 3;
				//現在どちらのターンかの判別
				bool turn;
				//プレイヤー生成
				Texture playerTex;
				Cube playerCube[2];

				//MovementRangeで確認する方向に合わせて確認する位置をずらすための配列
				//初期Directionは右にしたかったが、wallの選択と違う方式になっているためやむなく変える
				//また見る方向は反時計回りに変化します
				//また問題としてyを数値的にzとして扱うことになるので注意が必要
				Float2 SearchDirection[4] =
				{
								Float2(0.0f, 1.0f), //上
								Float2(-1.0f, 0.0f),//左
								Float2(0.0f,-1.0f), //下
								Float2(1.0f, 0.0f)  //右
				};
};