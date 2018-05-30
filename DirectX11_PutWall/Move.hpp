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