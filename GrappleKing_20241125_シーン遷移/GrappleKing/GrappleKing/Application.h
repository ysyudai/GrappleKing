#pragma once
#include "Geometry.h"

class Application
{
private:
	Application();
	// �֐��錾�̌�� = delete�ł��̊֐��̎g�p���֎~�ł���
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	Size windowSize_;

public:
	/// <summary>
	/// �V���O���g���I�u�W�F�N�g��Ԃ��֐�
	/// �Q�ƂŕԂ����ƂŁA������h��
	/// �ԈႦ�Ēl��Ԃ����ꍇ�A�R�s�[�R���X�g���N�^������̂ŁA
	/// �����private�ɂ��Ă������ƂŒe�����Ƃ��ł���
	/// </summary>
	/// <returns>Applicatoin.SingletonObject</returns>
	static Application& GetInstance();

	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <returns>true:����������, false:���s</returns>
	bool Init();

	void Run();

	void Terminate();

	const Size& GetWindowSize() const;
};

