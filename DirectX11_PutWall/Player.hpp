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

				//�ړ��ł��邩�̊m�F ������ 1 or -1 ��������Ă͂����Ȃ�
				void MoveConfirmation(int depth,int side)
				{
								//������ƒ����Ȃ肻���������̂ł����ŒZ�k
								Float3 move = Float3(movePosition.x + side, movePosition.y, movePosition.z + depth);
								if (move.x > Maxmove || move.z > Maxmove)
								{
												//�ړ��s�i�ړ��\����𒴂��Ă���j
												return;
								}
								if (wall.GetWallData(move.x,move.z,move.y))
								{
												//���̏ꏊ�Ƀu���b�N�����݂��Ă���
												if (wall.GetWallData(move.x, move.z, move.y + 1))
												{
																if (wall.GetWallData(move.x, move.z, move.y - 1))
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
				//�v���C���[�̕`��
				void Draw()
				{
								//�{�̂̕`��
								mesh.Draw(position, angles);
								//���ړ��̕`��i����ɂ͌�ŏ�����t����j
								if (true)
								{
												mesh.Draw(position + movePosition, angles);
								}
				}

private:
				Wall wall;
				Mesh mesh;
				//�`��p�̍��W
				Float3 position;
				Float3 angles;//�v��̂��s��
				//�ړ����鋗�����i�[����
				Float3 movePosition;//���̈ړ�������W��`�悷��̂Ɏg�p�i���ʂɂ��g�p�j
				const int Maxmove = 2;//�v���C���[����^�[���ɓ������Ƃ̂ł���}�X��
};