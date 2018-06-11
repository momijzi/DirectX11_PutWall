#pragma once
class Player
{
				Player(Texture* tex);
				
				~Player()
				{

				}

				//移動できるかの確認 ただし 1 or -1 しか入れてはいけない
				void MoveConfirmation(int depth, int side);
				
				//プレイヤーの描画
				void Draw();

private:
				Wall wall;
				Mesh mesh;
				//描画用の座標
				Float3 position;
				Float3 angles;//要るのか不明
																		//移動する距離を格納する
				Float3 movePosition;//仮の移動する座標を描画するのに使用（判別にも使用）
				const int Maxmove = 2;//プレイヤーが一ターンに動くことのできるマス数

};