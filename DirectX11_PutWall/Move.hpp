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

				//�T�C�R���̈ړ��̂悤�ȏ����@�J�^���J�^���� 
				//�ړ��X�s�[�h�͒m���@�܂����肵�Ă��Ȃ�
				//�Ƃ肠������]���ɂ߂荞�ޕ�������ɏ㏸������
				//void DiseMove()
				//{
				//				//��{�������g�p�����{�b�N�X�̉�]
				//				//�����90�x��]�̊�{���������@
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