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
				if (wall->GetBlockData(currentPos + Float3(0.0f,1.0f,0.0f)) != Wall::BlockType::NON)
				{
								upFlag = true;
				}
				//ここから索敵を開始する・・//Dirは索敵する方向 0が上と
						//ここで調べる必要のある方向かどうかを調べる
				for (int Dir = 0; Dir < 4; Dir++)
				{
								if (Dir != Direction)
								{
												if (wall->GetBlockData(currentPos +
																Float3(SearchDirection[Dir].x, 0, SearchDirection[Dir].y)) != Wall::BlockType::NON)
												{
																//調べた場所にブロックが存在していた・・これは上に行くチャンス
																//ただ調べた場所の上にブロックがあった時移動できない
																if ((int)wall->GetBlockData(currentPos +
																				Float3(SearchDirection[Dir].x, 0, SearchDirection[Dir].y)) != (int)turn + 1)
																{
																				//相手の出した特殊ブロックではないかどうかを調べる
																				//特殊ブロックを使用しないならコメントアウトで
																				if (!upFlag)
																				{
																								//上に何もなかったため調べる
																								if (wall->GetBlockData(currentPos +
																												Float3(SearchDirection[Dir].x, 1, SearchDirection[Dir].y)) == Wall::BlockType::NON)
																								{
																												//なんと上に行くことができることが判明した
																												wall->SetPlayerMoveFlag(currentPos +
																																Float3(SearchDirection[Dir].x, 1, SearchDirection[Dir].y));
																												if (moveCount != MaxMove)
																												{
																																//そして更なる先を求めて旅に出る・・
																																MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																				movePos.y + 1, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), moveCount + 1);
																												}
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
																				if (currentPos.y - j <	1)
																				{
																								//高さ　基点 - j + 1の位置に行くことができることが判明した
																								wall->SetPlayerMoveFlag(currentPos +
																												Float3(SearchDirection[Dir].x, -(float)j + 1.0f, SearchDirection[Dir].y));
																								if (moveCount != MaxMove)
																								{
																												//そして更なる先を求めて旅に出る・・
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y - (float)j + 1.0f, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), moveCount + 1);
																								}
																								break;
																				}
																				//ここに入る時点でその方向への移動ができることは確定なので床の部分にたどり着いた時は移動可能とする
																				if (wall->GetBlockData(currentPos + 
																								Float3(SearchDirection[Dir].x, -(float)j, SearchDirection[Dir].y)) != Wall::BlockType::NON)
																				{
																								//高さ　基点 - j + 1の位置に行くことができることが判明した
																								wall->SetPlayerMoveFlag(currentPos + 
																												Float3(SearchDirection[Dir].x, - (float)j + 1.0f, SearchDirection[Dir].y));
																								if (moveCount != MaxMove)
																								{
																												//そして更なる先を求めて旅に出る・・
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y - (float)j + 1.0f, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), moveCount + 1);
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
				for (int y = 1; y > -2; y--)
				{
								//ステージ外に行かないように制限
								//MaxHeight + 1はクリア条件の高さに達したかどうか
								if (player[turn].position.y + player[turn].movePosition.y + y > -1 &&
												player[turn].position.y + player[turn].movePosition.y + y < wall->MaxHeight + 1)
								{
												//横にブロックが存在した時は上にしか行けないようにする
												//またないときはfor(int i = 0;i < 2;i++)で同じ高さとその下を調べる
												if (wall->GetPlayerMoveFlag(player[turn].position+ player[turn].movePosition +
																Float3(SearchDirection[Direction].x, (float)y, SearchDirection[Direction].y)))
												{
																//移動できることが判明したので移動
																player[turn].movePosition += 
																				Float3(SearchDirection[Direction].x, (float)y, SearchDirection[Direction].y);
																return;
												}
								}
				}
}
bool PlayerManager::MoveFlagChack()
{
				//移動先に敵プレイヤーが存在しているか確認
				if (!App::SameChackFloat3(player[turn].position, player[!turn].position))
				{
								//居なかったので移動可能です
								player[turn].position += player[turn].movePosition;
								player[turn].movePosition = 0.0f;
								return true;
				}
				return false;
}
//
//void PlayerManager::ReturnMovePos()
//{
//				SetDrawFlag(true);
//				player[turn].position -= player[turn].movePosition;
//				player[turn].movePosition = 0.0f;
//}

bool PlayerManager::DownPlayer()
{
				bool flag = false;
				for (int i = 0; i < 2; i++)
				{
								player[i].position.y -= 1;
								if (player[i].position.y == 0)
								{
												flag = true;
								}
				}
				return flag;
}

void PlayerManager::DeliverLength(int length)
{
				player[turn].length -= length;
				if (player[!turn].length + plusPoint > 8)
				{
								player[!turn].length = 8;
				}
				else
				{
								player[!turn].length += plusPoint;
				}
}

void PlayerManager::Release(float mapLength)
{
				player[0].position = Float3(mapLength /2 - 1,1.0f, mapLength/ 2);
				player[0].movePosition = 0.0f;
				player[0].moveFlag = false;
				player[0].length = 2;
				player[1].position = Float3(mapLength / 2, 1.0f, mapLength / 2 - 1);
				player[1].movePosition = 0.0f;
				player[1].moveFlag = false;
				player[1].length = 2;
				turn = false;
}

void PlayerManager::Draw(int boxLength, int blockSize,float downPos)
{
				for (int i = 0; i < 2; i++)
				{
								playerCube[i].scale = (float)blockSize;
								playerCube[i].position = (player[i].position -
												Float3((float)boxLength / 2.0f, 0.0f, (float)boxLength / 2.0f) + 0.5f + player[i].movePosition) * (float)blockSize;
								playerCube[i].position.y -= downPos * (float)blockSize;
								playerCube[i].Draw();
				}
}

PlayerManager::Player::Player()
{
				position = Float3(0.0f,0.0f,0.0f);
				movePosition = Float3(0.0f, 0.0f, 0.0f);
				length = 0.0f;
}