#include "GameManager.h"
#include "PipeMovement.h"
#include "SoulEngine.h"

Ref<Font> roboto;
Transform* cameraTransform;
GameManager* GameManager::instance;

void GameManager::Create()
{
	if (instance != nullptr && instance != this)
	{
		Destroy();
		return;
	}

	roboto = FontManager::Load(R"(Assets\Fonts\Roboto-Regular.ttf)", 12);
	instance = this;
}

void GameManager::Update()
{
#ifdef _DEBUG
	if (Input::GetKeyUp(Keys::F10))
		Debug::showCollider = true;
#endif
	UI::DrawString(text, { 5.0f, 4.0f }, { 0.5f, 0.5f }, roboto, Color::Red());
}


void GameManager::SetLastTop(Transform& transform)
{
	if (!_currentTop) _currentTop = &transform;
	else _currentTop = _lastTop;

	_lastTop = &transform;
}

void GameManager::SetLastBottom(Transform& transform)
{
	if (!_currentBottom) _currentBottom = &transform;
	else _currentBottom = _lastBottom;
	_lastBottom = &transform;
}

void GameManager::RestartPipes()
{
	float index = 1.0f;
	float w = SceneManager::Active()->GetCamera()->GetWidth() + 1.5f;
	for (auto pipe : pipes)
	{
		pipe->GetTransform().position.x = w * index++;
		pipe->DestroyAndCreate();
	}
}