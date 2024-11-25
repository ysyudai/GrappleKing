#pragma once
#include <map>
#include <string>
#include <vector>

/// <summary>
/// ���͌n���R���g���[������N���X
/// </summary>
class Input
{
private:
	/// <summary>
	/// ���Ӌ@����
	/// </summary>
	enum class PeripheralType
	{
		keybd, // �L�[�{�[�h
		pad1 // �p�b�h1
	};
	// ���͏���`�p
	struct InputState
	{
		PeripheralType periType;
		int id; // ���͂�ID
	};

	using InputTable_t = std::map < std::string, std::vector<InputState>>;
	using InputRecord_t = std::map < std::string, bool>;

	// ���O�Ǝ��ۂ̓��͂̑Ή��\
	InputTable_t inputTable_;
	// �����ꂽ���ǂ������L�^���Ă�������
	InputRecord_t current_;
	InputRecord_t last_;

public:
	Input();

	/// <summary>
	/// ���͏�Ԃ��X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �������u�Ԃ����o
	/// </summary>
	/// <param name="id">���̓R�[�h</param>
	/// <returns>true:�������u�� false:�����ĂȂ�or�������ςȂ�</returns>
	bool IsTrigger(std::string key) const;
};

