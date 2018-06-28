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

//�v���C���[�����̃^�[���ړ����邱�Ƃ̂ł���ꏊ��ݒ�
//mocePos�����݂̍��W�̈ʒu����ǂꂾ���ω������n�_���猩��̂��̔���p
//Direction��������s������������//�E�ɍs������ɍēx���̈ʒu�𒲂ׂ�K�v�͂Ȃ���ŁE�E
//countMove�͌��݂ǂ̈ړ������Ă���̂��������@���݂�moveCount�ڂ̈ړ��ł���
//�ċA�g���܂�
void PlayerManager::MovementRange(Wall* wall, Float3 movePos, int Direction, int moveCount)
{
				//���̃X�R�[�v�ŕ����񈵂��̂ł����ł܂Ƃ߂�
				Float3 currentPos = Float3(player[turn].position.x + movePos.x,
								player[turn].position.y + movePos.y, player[turn].position.z + movePos.z);

				bool upFlag = false;//�v���C���[�̏�Ƀu���b�N�����݂��Ă�������ɏ�邱�Ƃ��ł��Ȃ��̂ŏ��������Ȃ�����
				if (wall->GetBlockData(currentPos + Float3(0.0f,1.0f,0.0f)) != Wall::BlockType::NON)
				{
								upFlag = true;
				}
				//����������G���J�n����E�E//Dir�͍��G������� 0�����
						//�����Œ��ׂ�K�v�̂���������ǂ����𒲂ׂ�
				for (int Dir = 0; Dir < 4; Dir++)
				{
								if (Dir != Direction)
								{
												if (wall->GetBlockData(currentPos +
																Float3(SearchDirection[Dir].x, 0, SearchDirection[Dir].y)) != Wall::BlockType::NON)
												{
																//���ׂ��ꏊ�Ƀu���b�N�����݂��Ă����E�E����͏�ɍs���`�����X
																//�������ׂ��ꏊ�̏�Ƀu���b�N�����������ړ��ł��Ȃ�
																if ((int)wall->GetBlockData(currentPos +
																				Float3(SearchDirection[Dir].x, 0, SearchDirection[Dir].y)) != (int)turn + 1)
																{
																				//����̏o��������u���b�N�ł͂Ȃ����ǂ����𒲂ׂ�
																				//����u���b�N���g�p���Ȃ��Ȃ�R�����g�A�E�g��
																				if (!upFlag)
																				{
																								//��ɉ����Ȃ��������ߒ��ׂ�
																								if (wall->GetBlockData(currentPos +
																												Float3(SearchDirection[Dir].x, 1, SearchDirection[Dir].y)) == Wall::BlockType::NON)
																								{
																												//�Ȃ�Ə�ɍs�����Ƃ��ł��邱�Ƃ���������
																												wall->SetPlayerMoveFlag(currentPos +
																																Float3(SearchDirection[Dir].x, 1, SearchDirection[Dir].y));
																												if (moveCount != MaxMove)
																												{
																																//�����čX�Ȃ������߂ė��ɏo��E�E
																																MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																				movePos.y + 1, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), moveCount + 1);
																												}
																								}
																				}
																}
												}
												else
												{
																//�����Ȃ������Ƃ������Ƃ͌���ێ�������Ƃ��������ނ��̓��
																//��ɉ����Ȃ��������ߒ��ׂ�
																for (int j = 1; j < 3; j++)
																{
																				if (currentPos.y - j <	1)
																				{
																								//�����@��_ - j + 1�̈ʒu�ɍs�����Ƃ��ł��邱�Ƃ���������
																								wall->SetPlayerMoveFlag(currentPos +
																												Float3(SearchDirection[Dir].x, -(float)j + 1.0f, SearchDirection[Dir].y));
																								if (moveCount != MaxMove)
																								{
																												//�����čX�Ȃ������߂ė��ɏo��E�E
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y - (float)j + 1.0f, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), moveCount + 1);
																								}
																								break;
																				}
																				//�����ɓ��鎞�_�ł��̕����ւ̈ړ����ł��邱�Ƃ͊m��Ȃ̂ŏ��̕����ɂ��ǂ蒅�������͈ړ��\�Ƃ���
																				if (wall->GetBlockData(currentPos + 
																								Float3(SearchDirection[Dir].x, -(float)j, SearchDirection[Dir].y)) != Wall::BlockType::NON)
																				{
																								//�����@��_ - j + 1�̈ʒu�ɍs�����Ƃ��ł��邱�Ƃ���������
																								wall->SetPlayerMoveFlag(currentPos + 
																												Float3(SearchDirection[Dir].x, - (float)j + 1.0f, SearchDirection[Dir].y));
																								if (moveCount != MaxMove)
																								{
																												//�����čX�Ȃ������߂ė��ɏo��E�E
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
								//�X�e�[�W�O�ɍs���Ȃ��悤�ɐ���
								//MaxHeight + 1�̓N���A�����̍����ɒB�������ǂ���
								if (player[turn].position.y + player[turn].movePosition.y + y > -1 &&
												player[turn].position.y + player[turn].movePosition.y + y < wall->MaxHeight + 1)
								{
												//���Ƀu���b�N�����݂������͏�ɂ����s���Ȃ��悤�ɂ���
												//�܂��Ȃ��Ƃ���for(int i = 0;i < 2;i++)�œ��������Ƃ��̉��𒲂ׂ�
												if (wall->GetPlayerMoveFlag(player[turn].position+ player[turn].movePosition +
																Float3(SearchDirection[Direction].x, (float)y, SearchDirection[Direction].y)))
												{
																//�ړ��ł��邱�Ƃ����������̂ňړ�
																player[turn].movePosition += 
																				Float3(SearchDirection[Direction].x, (float)y, SearchDirection[Direction].y);
																return;
												}
								}
				}
}
bool PlayerManager::MoveFlagChack()
{
				//�ړ���ɓG�v���C���[�����݂��Ă��邩�m�F
				if (!App::SameChackFloat3(player[turn].position, player[!turn].position))
				{
								//���Ȃ������̂ňړ��\�ł�
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