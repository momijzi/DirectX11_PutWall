#include"App.hpp"

Player::Player(Texture* tex)
{
				App::Initialize();
				position = Float3(0.0f, 0.0f, 0.0f);
				angles = Float3(0.0f, 0.0f, 0.0f);
				movePosition = Float3(0.0f, 0.0f, 0.0f);
}

//
void Player::MoveConfirmation(Wall* wall,int depth, int side)
{
				//ちょっと長くなりそうだったのでここで短縮
				Float3 move = Float3(movePosition.x + side, movePosition.y, movePosition.z + depth);
				if (move.x > Maxmove || move.z > Maxmove)
				{
								//移動不可（移動可能上限を超えている）
								return;
				}
				if (wall->GetWallData(move.x, move.z, move.y))
				{
								//その場所にブロックが存在していた
								if (wall->GetWallData(move.x, move.z, move.y + 1))
								{
												if (wall->GetWallData(move.x, move.z, move.y - 1))
												{
																//特に移動できる場所はなかった
																return;
												}
												movePosition += Float3(side, -1, depth);
												return;
								}
								movePosition += Float3(side, 1, depth);
								return;
				}
				movePosition += Float3(side, 0, depth);
				return;
}

void Player::Draw(bool virtualPlayerDraw)
{
				
				if (virtualPlayerDraw)
				{

				}
}