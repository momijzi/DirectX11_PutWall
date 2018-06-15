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
				if (wall->GetBlockData(currentPos.x, currentPos.z, currentPos.y + 1))
				{
								upFlag = true;
				}
				//����������G���J�n����E�E//Dir�͍��G������� 0�����
				for (int Dir = 0; Dir < 4; Dir++)
				{
								//�����Œ��ׂ�K�v�̂���������ǂ����𒲂ׂ�
								if (Dir != Direction)
								{
												if (wall->GetBlockData(currentPos.x + SearchDirection[Dir].x,
																currentPos.z + SearchDirection[Dir].y, currentPos.y))
												{
																//���ׂ��ꏊ�Ƀu���b�N�����݂��Ă����E�E����͏�ɍs���`�����X
																//�������ׂ��ꏊ�̏�Ƀu���b�N�����������ړ��ł��Ȃ�
																if (!upFlag)
																{
																				//��ɉ����Ȃ��������ߒ��ׂ�
																				if (!wall->GetBlockData(currentPos.x + SearchDirection[Dir].x,
																								currentPos.z + SearchDirection[Dir].y, currentPos.y + 1))
																				{
																								//�Ȃ�Ə�ɍs�����Ƃ��ł��邱�Ƃ���������
																								wall->SetPlayerMoveFlag(currentPos.x + SearchDirection[Dir].x,
																												currentPos.z + SearchDirection[Dir].y, currentPos.y + 1, true);
																								if (moveCount != MaxMove)
																								{
																												//�����čX�Ȃ������߂ė��ɏo��E�E
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y + 1, movePos.z + SearchDirection[Dir].y),(Dir ^ 2), moveCount + 1);
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
																				if (currentPos.y - j < 0)
																				{
																								//�����@��_ - j + 1�̈ʒu�ɍs�����Ƃ��ł��邱�Ƃ���������
																								wall->SetPlayerMoveFlag(currentPos.x + SearchDirection[Dir].x,
																												currentPos.z + SearchDirection[Dir].y, currentPos.y - j + 1, true);
																								if (moveCount != MaxMove)
																								{
																												//�����čX�Ȃ������߂ė��ɏo��E�E
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y - j + 1, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), moveCount + 1);
																								}
																								break;
																				}
																				//�����ɓ��鎞�_�ł��̕����ւ̈ړ����ł��邱�Ƃ͊m��Ȃ̂ŏ��̕����ɂ��ǂ蒅�������͈ړ��\�Ƃ���
																				if (wall->GetBlockData(currentPos.x + SearchDirection[Dir].x,
																								currentPos.z + SearchDirection[Dir].y, currentPos.y - j)
																								)
																				{
																								//�����@��_ - j + 1�̈ʒu�ɍs�����Ƃ��ł��邱�Ƃ���������
																								wall->SetPlayerMoveFlag(currentPos.x + SearchDirection[Dir].x,
																												currentPos.z + SearchDirection[Dir].y, currentPos.y - j + 1, true);
																								if (moveCount != MaxMove)
																								{
																												//�����čX�Ȃ������߂ė��ɏo��E�E
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
				//���ݒn�ɂ��߂��悤�ɂ���̂����E�E
				//����͉����C������E�E�ǂ��ɂ��C�������Ă����Ȃ��
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
								//�ړ�����}�X�ȏ�s���Ă���
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