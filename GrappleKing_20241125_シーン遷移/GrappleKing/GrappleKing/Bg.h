#pragma once

class Player;

class Bg
{
public:
	Bg(Player* pPlayer);  // �R���X�g���N�^
	~Bg(); // �f�X�g���N�^

	void Init();
	void Update();  // �X�V����
	void Draw();    // �`�揈��

private:
	// �O���t�B�b�N�̃n���h��
	int m_handle;

	// �O���t�B�b�N�Ƀ`�b�v�������܂܂�Ă��邩
	int m_graphChipNumX;
	int m_graphChipNumY;

	Player* m_player;

};