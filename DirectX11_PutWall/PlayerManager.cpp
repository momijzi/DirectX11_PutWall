#include"App.hpp"

PlayerManager::PlayerManager()
{
				App::Initialize();
				turn = false;
}

void PlayerManager::Behavior()
{
				
}

void PlayerManager::MovementRange(Wall* wall)
{
				bool upFlag = true;//�v���C���[�̏�Ƀu���b�N�����݂��Ă�������ɏ�邱�Ƃ��ł��Ȃ��̂ŏ��������Ȃ�����
				if (wall->GetBoxData(player[turn].position.x, player[turn].position.z, player[turn].position.y + 1))
				{
								upFlag = false;
				}
				//�O�㍶�E�s���邩�ǂ����̔��� �E���玞�v���Ɂ@�s����Ȃ����
				if (wall->GetBoxData(player[turn].position.x + 1, player[turn].position.z, player[turn].position.y))
				{
								if (upFlag)//��Ƀu���b�N�����邩�̊m�F
								{
												//��ɍs�����Ƃ��ł��邩�m�F
								}
				}
				else
				{
								//�����͓����̈ړ�
								if (wall->GetBoxData(player[turn].position.x + 1, player[turn].position.z, player[turn].position.y - 1))
								{
												
								}
				}
}

void PlayerManager::Release()
{
				turn = false;
}

PlayerManager::Player::Player()
{
				position = Float3(0.0f,0.0f,0.0f);
			 angles = Float3(0.0f, 0.0f, 0.0f);
				movePosition = Float3(0.0f, 0.0f, 0.0f);
}