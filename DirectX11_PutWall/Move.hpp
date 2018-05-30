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

				//‰Šú‚ÌˆÚ“®•ûŒü‚Í‰E‚Å‚ ‚é
				//move‚É‚ÍA0‚©‚P‚ğ“ü‚ê‚é
				//move.y‚ÍA‚»‚à‚»‚àc²‚ÌˆÚ“®‚ğ‚·‚é‚Ì‚©‚Ç‚¤‚©‚Ì‚½‚ß‚Ìˆ—
				Float3 MovePos(Float3 pos, bool Direction = true, bool x = false, bool y = false, bool z = false)
				{
								if (Direction) {
												pos += Float3
												(
																cos(DirectX::XMConvertToRadians(-angles.y + 90)) * z + cos(DirectX::XMConvertToRadians(-angles.y)) * x,//xÀ•W‚Ì‰ñ“]
																sin(DirectX::XMConvertToRadians(-angles.x)) * y,//yÀ•W‚Ì‰ñ“]
																sin(DirectX::XMConvertToRadians(-angles.y + 90)) * z + sin(DirectX::XMConvertToRadians(-angles.y)) * x //zÀ•W‚Ì‰ñ“]
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