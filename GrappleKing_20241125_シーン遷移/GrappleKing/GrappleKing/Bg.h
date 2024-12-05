#pragma once

class Player;

class Bg
{
public:
	Bg(Player* pPlayer);  // �R���X�g���N�^
	~Bg(); // �f�X�g���N�^

	void Update();  // �X�V����
	void Draw();    // �`�揈��

	int m_chipNo;

	bool m_isChipHit;

	bool IsWallHit;

	int GetChipLeft()const { return m_ChipLeft; }
	int GetChipRight()const { return m_ChipRight; }
	int GetChipTop()const { return m_ChipTop; }
	int GetChipBottom()const { return m_ChipBottom; }

	int GetChipTopGround()const { return m_chipTopGround; }
	int GetChipTopWall()const { return m_chipLeftWall; }
	int GetChipTopWall()const { return m_chipRightWall; }

private:
	// �O���t�B�b�N�̃n���h��
	int m_handle;
	int m_bgHandle;

	// �O���t�B�b�N�Ƀ`�b�v�������܂܂�Ă��邩
	int m_graphChipNumX;
	int m_graphChipNumY;

	Player* m_player;

	void Map();

	int m_ChipLeft;
	int m_ChipRight;
	int m_ChipTop;
	int m_ChipBottom;

	int m_chipTopGround;
	int m_chipLeftWall;
	int m_chipRightWall;
};