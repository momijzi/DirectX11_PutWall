#pragma once

class PlayerManager
{
public:
				PlayerManager();
				~PlayerManager() {};

				//プレイヤーの行動　引数にどちらのターンかをboolで入れる
				void Behavior();
				//プレイヤーがこのターン移動できる場所を検索
				void MovementRange(Wall* wall, Float3 movePos = Float3(0.0f,0.0f,0.0f),
								int Direction = -1, int moveCount = 1);

				//次のプレイヤーのターンにする
				void NextTurn() { turn = !turn; }

				//移動が完了していたら
				void MoveableChack(Wall* wall, int Direction);
				
				//一マス以上の移動をしているか
				bool MoveFlagChack();

				void Draw(int boxLength)
				{
								for (int i = 0; i < 2; i++)
								{
												playerCube[i].position = player[i].position;
												playerCube[i].Draw();
								}
				}

				//リプレイ時に初期地点に戻すために使う
				void Release(Float3 positionA = Float3(3.0, 0.0f, 4.0f),
								Float3 positionB = Float3(4.0, 0.0f, 3.0f));

private:
				struct Player
				{
								Player();
								//描画用の座標
								Float3 position;
								//仮の移動する座標を描画するのに使用（判別にも使用）
								Float3 movePosition;
				};
				Player player[2];
				//プレイヤーが一ターンに動くことのできるマス数
				const int MaxMove = 2;
				//現在どちらのターンかの判別
				bool turn;

				Texture playerTex;
				Cube playerCube[2];

				//MovementRangeで確認する方向に合わせて確認する位置をずらすための配列
				//初期Directionは右です//また見る方向は反時計回りに変化します
				//また問題としてyをzとして扱うことになるので注意が必要
				Float2 SearchDirection[4] =
				{
								Float2(1.0f, 0.0f), //右
								Float2(0.0f, 1.0f), //上
								Float2(-1.0f, 0.0f),//左
								Float2(0.0f,-1.0f) //下
				};
};