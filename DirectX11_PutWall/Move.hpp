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

				//�����̈ړ������͉E�ł���
				//move�ɂ́A0���P������
				//move.y�́A���������c���̈ړ�������̂��ǂ����̂��߂̏���
				Float3 MovePos(Float3 pos, bool Direction = true, bool x = false, bool y = false, bool z = false)
				{
								if (Direction) {
												pos += Float3
												(
																cos(DirectX::XMConvertToRadians(-angles.y + 90)) * z + cos(DirectX::XMConvertToRadians(-angles.y)) * x,//x���W�̉�]
																sin(DirectX::XMConvertToRadians(-angles.x)) * y,//y���W�̉�]
																sin(DirectX::XMConvertToRadians(-angles.y + 90)) * z + sin(DirectX::XMConvertToRadians(-angles.y)) * x //z���W�̉�]
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