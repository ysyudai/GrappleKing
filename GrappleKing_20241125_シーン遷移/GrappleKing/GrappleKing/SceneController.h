#pragma once
// �X�}�|�g��
#include <memory>
#include <list>

class Input;
class Scene;

/// <summary>
/// �e�V�[�����Ǘ�����N���X
/// �Ǘ��͂��邪�A�J�ڂ��Ǘ��͂��Ȃ�
/// �C���^�[�t�F�C�X��񋟂���N���X
/// </summary>
class SceneController
{
private:
	std::list<std::shared_ptr<Scene>> scenes_; // ���ݎ��s���̃V�[�� �X�^�b�N�\��

public:
	SceneController();


	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Update�����̂܂܎��s�B
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input);
	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Draw�����̂܂܎��s�B
	/// </summary>
	void Draw();
	/// <summary>
	/// ���̏�Ԃ��Z�b�g����(�Z�b�g����̂͊e��Ԃ̖���)
	/// �e�V�[�����R���g���[���[�����Đ؂�ւ�������H
	/// </summary>
	/// <param name="">���̏�Ԃ̃|�C���^�i�X�}�|�j</param>
	void ChangeScene(std::shared_ptr<Scene>);

	/// <summary>
	/// ���ݎ��s���̃V�[���̏�ɕʂ̃V�[�����������
	/// </summary>
	/// <param mane="scene">����������V�[��</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// ���݁A��ԏ�ɂ�����s���̃V�[������菜��
	/// </summary>
	void PopScene();
};

