#include"App.hpp"

PlayerManager::PlayerManager()
{
				App::Initialize();
				turn = false;

				Release();
}

void PlayerManager::Behavior()
{
				
}
//�v���C���[�����̃^�[���ړ����邱�Ƃ̂ł���ꏊ��ݒ�
//mocePos�����݂̍��W�̈ʒu����ǂꂾ���ω������n�_���猩��̂��̔���p
//Direction��������s������������//�E�ɍs������ɍēx���̈ʒu�𒲂ׂ�K�v�͂Ȃ���ŁE�E
//flag�͂��̃��[�v�̒��ł���ɉ��[�����ׂ�K�v������̂��ǂ����A����͊K�w�P�����������Ȃ��̂�bool�Ŕ��f
//�ċA�g���܂�
void PlayerManager::MovementRange(Wall* wall, Float3 movePos, int Direction, bool flag)
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
																								if (flag)
																								{
																												//�����čX�Ȃ������߂ė��ɏo��E�E
																												MovementRange(wall, Float3(currentPos.x + SearchDirection[Dir].x,
																																currentPos.y + 1, currentPos.z + SearchDirection[Dir].y), (Dir ^ 2), false);
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
																				//�����ɓ��鎞�_�ł��̕����ւ̈ړ����ł��邱�Ƃ͊m��Ȃ̂ŏ��̕����ɂ��ǂ蒅�������͈ړ��\�Ƃ���
																				if (wall->GetBlockData(currentPos.x + SearchDirection[Dir].x,
																								currentPos.z + SearchDirection[Dir].y, currentPos.y - j) ||
																								currentPos.y - j < 0)
																				{
																								//�����@��_ - j�̈ʒu�ɍs�����Ƃ��ł��邱�Ƃ���������
																								wall->SetPlayerMoveFlag(currentPos.x + SearchDirection[Dir].x,
																												currentPos.z + SearchDirection[Dir].y, currentPos.y - j + 1, true);
																								if (flag)
																								{
																												//�����čX�Ȃ������߂ė��ɏo��E�E
																												MovementRange(wall, Float3(movePos.x + SearchDirection[Dir].x,
																																movePos.y - j + 1, movePos.z + SearchDirection[Dir].y), (Dir ^ 2), false);
																								}
																								break;
																				}
																}
												}
								}
				}
}
void PlayerManager::Release(Float3 positionA, Float3 positionB)
{
				player[0].position = positionA;
				player[0].angles = 0.0f;
				player[0].movePosition = 0.0f;
				player[1].position = positionB;
				player[1].angles = 0.0f;
				player[1].movePosition = 0.0f;

				turn = false;
}

PlayerManager::Player::Player()
{
				position = Float3(0.0f,0.0f,0.0f);
			 angles = Float3(0.0f, 0.0f, 0.0f);
				movePosition = Float3(0.0f, 0.0f, 0.0f);
}