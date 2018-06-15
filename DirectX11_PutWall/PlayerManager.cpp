#include"App.hpp"

PlayerManager::PlayerManager()
{
				App::Initialize();
				turn = false;

				playerTex.Load(L"texture/playerTex.png");
				playerTex.SetDivide(Float2(2.0f, 1.0f));
				playerCube[0].Create(&playerTex, 1);
				playerTex.SetNumUvAll(Float2(1.0f,0.0f));
				playerCube[1].Create(&playerTex, 1);

				Release();
}

void PlayerManager::Behavior()
{
				
}
//プレイヤーがこのターン移動することのできる場所を設定
//mocePosが現在の座標の位置からどれだけ変化した地点から見るのかの判定用
//Directionが判定を行う方向を示す//右に行った後に再度元の位置を調べる必要はないんで・・
//countMoveは現在どの移動をしているのかを示す　現在はmoveCount目の移動である
//再帰使います
void PlayerManager::MovementRange(Wall* wall, Float3 movePos, int Direction, int moveCount)
{
				//このスコープで複数回扱うのでここでまとめる
				Float3 currentPos = Float3(player[turn].position.x + movePos.x,
								player[turn].position.y + movePos.y, player[turn].position.z + movePos.z);

				bool upFlag = false;//プレイヤーの上にブロックが存在していた時上に上ることができないので処理を少なくする
				if (wall->GetBlockData(currentPos.x, currentPos.z, currentPos.y + 1))
				{
								upFlag = true;
				}
				//ここから索敵を開始する・・//Dirは索敵する方向 0が上と
				for (int Dir = 0; Dir < 4; Dir++)
				{
								//ここで調べる必要のある方向かどうかを調べる
								if (Dir != Direction)
								{
												if (wall->GetBlockData(currentPos.x + SearchDirection[Dir].x,
																currentPos.z + SearchDirection[Dir].y, currentPos.y))
												{
																//調べた場所にブロックが存在していた・・これは上に行くチャンス
																//ただ調べた場所の上にブロックがあった時移動できない
																if (!upFlag)
																{
																				//上に何もなかったため調べる
																				if (!wall->GetBlockData(currentPos.x + SearchDirection[Dir].x,
																								currentPos.z + SearchDirection[Dir].y, currentPos.y + 1))
																				{
																								//なんと上に行くことができることが判明した
																								wall->SetPlayerMoveFlag(currentPos.x + SearchDirection[Dir].x,
																												currentPos.z + SearchDirection[Dir].y, currentPos.y + 1, true);
																								if (moveCount != MaxMove)
																								{
																												//そして更なる先を求めて旅に出る・・
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y + 1, movePos.z + SearchDirection[Dir].y),(Dir ^ 2), moveCount + 1);
																								}
																				}
																}
												}
												else
												{
																//何もなかったということは現状維持かそれとも落ち込むかの二択
																//上に何もなかったため調べる
																for (int j = 1; j < 3; j++)
																{
																				if (currentPos.y - j < 0)
																				{
																								//高さ　基点 - j + 1の位置に行くことができることが判明した
																								wall->SetPlayerMoveFlag(currentPos.x + SearchDirection[Dir].x,
																												currentPos.z + SearchDirection[Dir].y, currentPos.y - j + 1, true);
																								if (moveCount != MaxMove)
																								{
																												//そして更なる先を求めて旅に出る・・
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y - j + 1, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), moveCount + 1);
																								}
																								break;
																				}
																				//ここに入る時点でその方向への移動ができることは確定なので床の部分にたどり着いた時は移動可能とする
																				if (wall->GetBlockData(currentPos.x + SearchDirection[Dir].x,
																								currentPos.z + SearchDirection[Dir].y, currentPos.y - j)
																								)
																				{
																								//高さ　基点 - j + 1の位置に行くことができることが判明した
																								wall->SetPlayerMoveFlag(currentPos.x + SearchDirection[Dir].x,
																												currentPos.z + SearchDirection[Dir].y, currentPos.y - j + 1, true);
																								if (moveCount != MaxMove)
																								{
																												//そして更なる先を求めて旅に出る・・
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y - j + 1, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), moveCount + 1);
																								}
																								break;
																				}
																}
												}
								}
				}
}

void PlayerManager::MoveableChack(Wall* wall, int Direction)
{
				//現在地にも戻れるようにするのだが・・
				//これは汚い気がする・・どうにか修正をしていくなりよ
				if (player[turn].position.x + player[turn].movePosition.x + SearchDirection[Direction].x == player[turn].position.x &&
								player[turn].position.z + player[turn].movePosition.z + SearchDirection[Direction].y == player[turn].position.z &&
								player[turn].position.y + player[turn].movePosition.y - player[turn].position.y <  2 &&
								player[turn].position.y + player[turn].movePosition.y - player[turn].position.y > -2)
				{
								player[turn].movePosition = 0.0f;
								return;
				}
				for (int y = -1; y < 2; y++)
				{
								if (player[turn].position.y + player[turn].movePosition.y + y > -1)
								{
												if (wall->GetPlayerMoveFlag(
																player[turn].position.x + player[turn].movePosition.x + SearchDirection[Direction].x,
																player[turn].position.z + player[turn].movePosition.z + SearchDirection[Direction].y,
																player[turn].position.y + player[turn].movePosition.y + y))
												{
																player[turn].movePosition +=
																				Float3(
																								SearchDirection[Direction].x,
																								y,
																								SearchDirection[Direction].y
																				);
																return;
												}
								}
				}
}
bool PlayerManager::MoveFlagChack()
{
				if (fabs(player[turn].movePosition.x) + fabs(player[turn].movePosition.y) + fabs(player[turn].movePosition.z) != 0 &&
								(player[turn].movePosition.x + player[turn].position.x != player[!turn].position.x ||
												player[turn].movePosition.y + player[turn].position.y != player[!turn].position.y ||
												player[turn].movePosition.z + player[turn].position.z != player[!turn].position.z))
				{
								//移動を一マス以上行っている
								player[turn].position += player[turn].movePosition;
								return true;
				}
				return false;
}

void PlayerManager::ReturnMovePos()
{
				player[turn].position -= player[turn].movePosition;
				player[turn].movePosition = 0.0f;
}

void PlayerManager::DeliverLength(int length)
{
				player[turn].length -= length;
				player[!turn].length += length;
}

void PlayerManager::Release(Float3 positionA, Float3 positionB)
{
				player[0].position = positionA;
				player[0].movePosition = 0.0f;
				player[0].moveFlag = false;
				player[0].length = 6.0f;
				player[1].position = positionB;
				player[1].movePosition = 0.0f;
				player[1].moveFlag = false;
				player[1].length = 6.0f;

				turn = false;
}

void PlayerManager::Draw(int boxLength, int blockSize)
{
				for (int i = 0; i < 2; i++)
				{
								playerCube[i].scale = blockSize;
								playerCube[i].position = (player[i].position -
												Float3(boxLength / 2, 0, boxLength / 2) + 0.5f) * blockSize;
								playerCube[i].Draw();
								if (player[i].moveFlag)
								{
												playerCube[i].position = (player[i].position + player[i].movePosition -
																Float3(boxLength / 2, 0, boxLength / 2) + 0.5f) * blockSize;
												playerCube[i].Draw();
								}
				}
}

PlayerManager::Player::Player()
{
				position = Float3(0.0f,0.0f,0.0f);
				movePosition = Float3(0.0f, 0.0f, 0.0f);
}