#pragma once
class Player
{
				Player(Texture* tex);
				
				~Player()
				{

				}

				//移動できるかの確認 ただし 1 or -1 しか入れてはいけない
				void MoveConfirmation(Wall* wall,int depth, int side);
				
				//プレイヤーの描画 //移動先の描画をするかしないかを判定　基本描画しない
				void Draw(bool virtualPlayerDraw = false);

private:

				//描画用の座標
				Float3 position;
				Float3 angles;//要るのか不明//プレイヤーがボックスより円状で小さい場合使用する
			//移動する距離を格納する
				Float3 movePosition;//仮の移動する座標を描画するのに使用（判別にも使用）
				const int Maxmove = 2;//プレイヤーが一ターンに動くことのできるマス数
};