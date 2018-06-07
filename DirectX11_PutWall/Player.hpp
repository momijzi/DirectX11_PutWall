class Player
{
public:
				Player(Texture* tex)
				{
								App::Initialize();
								position = Float3(0.0f, 0.0f, 0.0f);
								angles = Float3(0.0f, 0.0f, 0.0f);
								movePosition = Float3(0.0f, 0.0f, 0.0f);
								mesh.CreateData(tex, Mesh::CreateMode::CUBEOUT);
				}
				~Player()
				{
								
				}

				//移動できるかの確認 ただし 1 or -1 しか入れてはいけない
				void MoveConfirmation(int depth,int side)
				{
								//ちょっと長くなりそうだったのでここで短縮
								Float3 move = Float3(movePosition.x + side, movePosition.y, movePosition.z + depth);
								if (move.x > Maxmove || move.z > Maxmove)
								{
												//移動不可（移動可能上限を超えている）
												return;
								}
								if (wall.GetWallData(move.x,move.z,move.y))
								{
												//その場所にブロックが存在していた
												if (wall.GetWallData(move.x, move.z, move.y + 1))
												{
																if (wall.GetWallData(move.x, move.z, move.y - 1))
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
				//プレイヤーの描画
				void Draw()
				{
								//本体の描画
								mesh.Draw(position, angles);
								//仮移動の描画（これには後で条件を付ける）
								if (true)
								{
												mesh.Draw(position + movePosition, angles);
								}
				}

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