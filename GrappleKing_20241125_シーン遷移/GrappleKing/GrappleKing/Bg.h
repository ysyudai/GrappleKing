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
private:
	// �O���t�B�b�N�̃n���h��
	int m_handle;
	int m_bgHandle;

	// �O���t�B�b�N�Ƀ`�b�v�������܂܂�Ă��邩
	int m_graphChipNumX;
	int m_graphChipNumY;

	Player* m_player;

	void Map();
};