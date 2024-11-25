#pragma once

class Input;
class SceneController;

class Scene
{
protected:
	// Šeó‘Ô‚ªØ‚è‘Ö‚¦‚é‚½‚ß‚É
	SceneController& controller_;

public:
	Scene(SceneController& cont);
	virtual ~Scene() {};

	/// <summary>
	/// “à•”•Ï”‚ÌXV
	/// </summary>
	/// <param name="input">“ü—Íó‘Ô</param>
	virtual void Update(Input& input) = 0;

	/// <summary>
	///  •`‰æ
	/// </summary>
	virtual void Draw() = 0;
};

