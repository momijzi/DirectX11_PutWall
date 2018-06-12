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
				//������ƒ����Ȃ肻���������̂ł����ŒZ�k
				Float3 move = Float3(movePosition.x + side, movePosition.y, movePosition.z + depth);
				if (move.x > Maxmove || move.z > Maxmove)
				{
								//�ړ��s�i�ړ��\����𒴂��Ă���j
								return;
				}
				if (wall->GetWallData(move.x, move.z, move.y))
				{
								//���̏ꏊ�Ƀu���b�N�����݂��Ă���
								if (wall->GetWallData(move.x, move.z, move.y + 1))
								{
												if (wall->GetWallData(move.x, move.z, move.y - 1))
												{
																//���Ɉړ��ł���ꏊ�͂Ȃ�����
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