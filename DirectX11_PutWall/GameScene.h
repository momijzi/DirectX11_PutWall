#pragma once
class GameScene
{
public:
				GameScene();
				~GameScene() {};

				void SceneManager();

				void MainTurn();

				//�J�����̊p�x���}�E�X�ŕύX����
				//����̓J�����}�l�[�W���[�ŏ����ׂ���������͑Ë�
				void CameraAnglesChangeMouse();
				//�J�����̍��W�ړ�
				void CameraPositionMove();
				//�ړ����Ă���Ƃ����̈ړ��͂ǂ̕����ɍs���Ă��邩���m�F����
				int KeyMoveData();
				//������
				void Release();

private:
				//�Q�[���̗���
				enum GameState { TITLE, CONFIG, PLAY, OVER };
				GameState gameState = TITLE;

				//�v���C���̊�{�I�ȗ���
				enum Scene { TURN_FIRST, PLAYER_MOVE, PUSH_WALL_SELECT, SET_PUSH_WALL_LENGTH,
								CAMERA_MOVE_CHACK, TURN_END };
				Scene scene = TURN_FIRST;

				//�J��������
				//3D��Ԃ�`��Ɏg�p����J����
				Camera mainCamera;
				//�ǂƒ��̃u���b�N����ɕ`�悷��
				Wall wall;
				//�v���C���[�̈ړ��Ȃǂ̏������s��
				PlayerManager playerManager;
				//UI��`�悷��Ƃ��Ɏg�p����@���������g�Ő錾����UI�`�悷�邾��
				UIData uiData;
				//�J�����̈ړ����s���i���S�ł͂Ȃ����������炩���������̂Łj
				Move move;

				//Wall�̉����o�����s�������̎����p�Ɏg�p
				int testSurface = 0;
				int testWidth = 3;
				int testHeight = 4;
				int testLength = 5;
				Float3 testPos[5] = {};
				bool testFlag = false;
				float testCount = 0.0f;

				
};