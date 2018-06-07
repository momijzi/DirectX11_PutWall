class Move
{
public:
				Float3 position;
				Float3 angles;

				const float speed = 0.1f;

				Move()
				{
								App::Initialize();
				}
				~Move() {}

				//サイコロの移動のような処理　カタンカタンと 
				//移動スピードは知らん　まだ特定していない
				//とりあえず回転時にめり込む分だけ上に上昇させる
				//void DiseMove()
				//{
				//				//基本周期を使用したボックスの回転
				//				//これで90度回転の基本周期完了　
				//
				//		mesh.angles.z += 1.0f;
				//		if (mesh.angles.z >= 90.0f)
				//		{
				//					mesh.angles.z = 0.0f;
				//		}
				//		mesh.position.y = sin(2 * DirectX::XMConvertToRadians(mesh.angles.z));
				//		mesh.Draw(mesh.position, mesh.angles);
				//		mesh2.Draw(mesh2.position, mesh2.angles);
				//}

				//初期の移動方向は右である
				//moveには、0か１を入れる
				//move.yは、そもそも縦軸の移動をするのかどうかのための処理
				Float3 MovePos(Float3 pos, bool Direction = true, bool x = false, bool y = false, bool z = false)
				{
								if (Direction) {
												pos += Float3
												(
																cos(DirectX::XMConvertToRadians(-angles.y + 90)) * z + cos(DirectX::XMConvertToRadians(-angles.y)) * x,//x座標の回転
																sin(DirectX::XMConvertToRadians(-angles.x)) * y,//y座標の回転
																sin(DirectX::XMConvertToRadians(-angles.y + 90)) * z + sin(DirectX::XMConvertToRadians(-angles.y)) * x //z座標の回転
												) * speed;
								}
								else
								{
												pos -= Float3
												(
																cos(DirectX::XMConvertToRadians(-angles.y + 90)) * z + cos(DirectX::XMConvertToRadians(-angles.y)) * x,
																sin(DirectX::XMConvertToRadians(-angles.x)) * y,
																sin(DirectX::XMConvertToRadians(-angles.y + 90)) * z + sin(DirectX::XMConvertToRadians(-angles.y)) * x
												) * speed;
								}
								return pos;
				}
};