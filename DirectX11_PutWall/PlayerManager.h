#pragma once

class PlayerManager
{
public:
				PlayerManager();
				~PlayerManager() {};
				
				//プレイヤーの行動　引数にどちらのターンかをboolで入れる
				void Behavior();
				void MovementRange(Wall* wall);
				//リプレイ時に初期地点に戻すために使う
				void Release();

private:
				struct Player
				{
								Player();
								Float3 position;//描画用の座標
								Float3 angles;//要るのか不明//プレイヤーがボックスより円状で小さい場合使用する
								Float3 movePosition;//仮の移動する座標を描画するのに使用（判別にも使用）
				};
				Player player[2];
				const int Maxmove = 2;//プレイヤーが一ターンに動くことのできるマス数
				bool turn;//現在どちらのターンかの判別

};