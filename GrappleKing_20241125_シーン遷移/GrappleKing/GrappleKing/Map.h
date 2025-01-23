#pragma once

class Player;
class Map
{
public:
	Map(Player* pPlayer);  // �R���X�g���N�^
	~Map(); // �f�X�g���N�^

	void Update();  // �X�V����
	void Draw();    // �`�揈��

	int m_chipNo;

	bool m_isGroundHit;

	bool IsWallHit;

	bool IsCeilingHit;

	int GetChipLeft()const { return m_chipLeft; }
	int GetChipRight()const { return m_chipRight; }
	int GetChipTop()const { return m_chipTop; }
	int GetChipBottom()const { return m_chipBottom; }

	int GetChipTopGround()const { return m_chipTopGround; }
	int GetChipLeftWall()const { return m_chipLeftWall; }
	int GetChipRightWall()const { return m_chipRightWall; }
	int GetChipBottomCeiling()const { return m_chipBottomCeiling; }

	bool IsStage1;
	bool IsStage2;
	bool IsStage3;

private:
	// �O���t�B�b�N�̃n���h��
	int m_handle;
	int m_backHandle;
	int m_cloudsHandle[4];
	
	// �O���t�B�b�N�Ƀ`�b�v�������܂܂�Ă��邩
	int m_graphChipNumX;
	int m_graphChipNumY;

	Player* m_player;

	void MapUpdate();

	int m_chipLeft;
	int m_chipRight;
	int m_chipTop;
	int m_chipBottom;

	int m_chipTopGround;
	int m_chipLeftWall;
	int m_chipRightWall;
	int m_chipBottomCeiling;

	//Vec2 m_cloudsPos[4];
	
};