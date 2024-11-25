#include"Pad.h"
#include"DxLib.h"

namespace
{
	int padInput = 0;	//���݂̃t���[���̃p�b�h����
	int lastInput = 0;	//�ЂƂO�̃t���[���̃p�b�h����
}

namespace Pad
{
	void Update()
	{
		//�O�̃t���[���ɉ�����Ă����{�^���̏����o���Ă���
		lastInput = padInput;
		//���̃t���[���ɉ�����Ă����{�^���̏����擾����
		padInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

#if false
	// Press����̎擾
	// �߂�l:������Ă�����true,������Ă��Ȃ�������false
	// ����:������s�������{�^��
	bool IsPress(int button)
	{
		if (padInput & button)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// Trigger����̎擾
	bool IsTrigger(int button)
	{
		//�����������݂̃t���[���Ƀ{�^����������Ă��Ȃ�
		if ( !(padInput & button) )
		{
			return false;
		}

		//�����܂ŏ����������Ƃ������Ƃ�
		//���̃t���[���Ƀ{�^����������Ă���

		//�O�̃t���[���Ƀ{�^����������Ă��Ȃ����Trigger�Ɣ���ł���
		if (lastInput & button)
		{
			return false;
		}
		else
		{
			//�O�̃t���[���ɉ�����Ă��Ȃ���
			//���݂̃t���[���ɉ�����Ă���Ƃ������Ƃ�
			//���̃t���[���������ꂽ�u��
			return true;
		}
	}
#else
	bool IsPress(int button)
	{
		return(padInput & button);
	}

	bool IsTrigger(int button)
	{
		// (padInput & button) == 0�̏ꍇfalse,
		//����ȊO�̏ꍇ��true������
		bool isNow = (padInput & button);
		bool isLast = (lastInput & button);
		return(isNow && !isLast);
	}
#endif
}