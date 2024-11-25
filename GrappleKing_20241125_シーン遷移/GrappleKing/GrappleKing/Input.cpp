#include "Input.h"
#include <DxLib.h>
#include <stdexcept>

// �J�b�R�ɉ��������ĂȂ��Ƃ���0�ŏ���������Ă�
Input::Input()
{
	// ���͂Ɩ��O�̑Ή��\�����
	inputTable_["next"] = { {PeripheralType::keybd, KEY_INPUT_RETURN},
							{PeripheralType::pad1, PAD_INPUT_A} };
	inputTable_["pause"] = { {PeripheralType::keybd, KEY_INPUT_P},
							{PeripheralType::pad1, PAD_INPUT_START} };
}

void Input::Update()
{
	// �O��̃t���[���̓��͂�����Ă���
	last_ = current_;

	char keyState[256];
	int padInput = GetJoypadInputState(DX_INPUT_PAD1);
	// ���͂��Ƃ�
	GetHitKeyStateAll(keyState);
	// �e�[�u���ɋL�����ꂽ���ׂĂ̓��͂��`�F�b�N����
	for (const auto& keyValue : inputTable_)
	{
		// �o�Ă����L�[�C�x���g�ɑ΂��A
		const auto& key = keyValue.first;
		current_[key] = false;
		// ���ꂼ��̓��͌`���Ń`�F�b�N����
		for (const auto& input : keyValue.second)
		{
			switch (input.periType)
			{
			case PeripheralType::keybd:
				current_[keyValue.first] = current_[key] || keyState[input.id];
				break;
			case PeripheralType::pad1:
				current_[keyValue.first] = current_[key] || (input.id & padInput);
				break;
			default:
				break;
			}
		}
	}
}

bool Input::IsTrigger(std::string key) const
{
	if (!inputTable_.contains(key))
	{
		throw std::runtime_error("�L�[�����݂��Ȃ�");
	}

	// id���O�񉟂���Ă��Ȃ��āA���݉�����Ă��鎞��true��Ԃ�
	// []���Z�q�ł͂Ȃ��Aat(key)���g�����ƁI�I�I�I�I
	return (current_.at(key) && !last_.at(key));
}
