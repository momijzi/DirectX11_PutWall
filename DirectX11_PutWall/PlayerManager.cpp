#include"App.hpp"

PlayerManager::PlayerManager()
{
				App::Initialize();
				turn = false;
}

void PlayerManager::Behavior()
{
				
}

void PlayerManager::MovementRange(Wall* wall)
{
				bool upFlag = true;//プレイヤーの上にブロックが存在していた時上に上ることができないので処理を少なくする
				if (wall->GetBoxData(player[turn].position.x, player[turn].position.z, player[turn].position.y + 1))
				{
								upFlag = false;
				}
				//前後左右行けるかどうかの判定 右から時計回りに　行けるなら入る
				if (wall->GetBoxData(player[turn].position.x + 1, player[turn].position.z, player[turn].position.y))
				{
								if (upFlag)//上にブロックがあるかの確認
								{
												//上に行くことができるか確認
								}
				}
				else
				{
								//高さは同じの移動
								if (wall->GetBoxData(player[turn].position.x + 1, player[turn].position.z, player[turn].position.y - 1))
								{
												
								}
				}
}

void PlayerManager::Release()
{
				turn = false;
}

PlayerManager::Player::Player()
{
				position = Float3(0.0f,0.0f,0.0f);
			 angles = Float3(0.0f, 0.0f, 0.0f);
				movePosition = Float3(0.0f, 0.0f, 0.0f);
}