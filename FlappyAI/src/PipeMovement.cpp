#include "PipeMovement.h"
#include "GameManager.h"
#include "SoulEngine.h"

MainCamera* mainCamera;

void PipeMovement::Create()
{
	mainCamera = SceneManager::Active()->GetCamera();
	
	auto scene = SceneManager::Active();
	pipe1 = scene->CreateGameObject("P");
	pipe2 = scene->CreateGameObject("P");
	auto& renderer1 = pipe1.AddComponent<SpriteRenderer>(); renderer1.color = { 0.8f, 0.6f, 0.2f, 1.0f };
	auto& renderer2 = pipe2.AddComponent<SpriteRenderer>(); renderer2.color = { 0.8f, 0.6f, 0.2f, 1.0f };
	auto& boxCollider1 = pipe1.AddComponent<BoxCollider2D>(); boxCollider1.SetTrigger(true);
	auto& boxCollider2 = pipe2.AddComponent<BoxCollider2D>(); boxCollider2.SetTrigger(true);
	rigidbody1 = &pipe1.AddComponent<Rigidbody2D>(); rigidbody1->SetBodyType(BodyType::Kinematic);
	rigidbody2 = &pipe2.AddComponent<Rigidbody2D>(); rigidbody2->SetBodyType(BodyType::Kinematic);
	pipe1.AddComponent<IgnoreSerialize>();
	pipe2.AddComponent<IgnoreSerialize>();

	DestroyAndCreate();
}

void PipeMovement::Start()
{
}

void PipeMovement::Update()
{
	if (pipe1.GetTransform().position.x < GameManager::instance->birdX)
		DestroyAndCreate();
}

void PipeMovement::FixedUpdate()
{
	rigidbody1->SetVelocity(GameManager::instance->pipeSpeed, 0.0f);
	rigidbody2->SetVelocity(GameManager::instance->pipeSpeed, 0.0f);
}

void PipeMovement::DestroyAndCreate()
{
	CreatePipes();
}

void PipeMovement::CreatePipes()
{
	auto scene = SceneManager::Active();
	float h = mainCamera->GetHeight();
	float hh = h + h;

	float size = Random::Next(GameManager::instance->pipeMinSize, h * 2.0f - GameManager::instance->pipeMinSize);
	float gapHalf = GameManager::instance->pipeGapSize / 2.0f;
	float sizeFirst = size - gapHalf;
	float sizeSecond = hh - size - gapHalf;

	auto& transform = GetTransform();
	auto& pipe1Transform = pipe1.GetTransform();
	auto& pipe2Transform = pipe2.GetTransform();

	pipe1Transform.position.x = transform.position.x; pipe1Transform.position.y = (h - sizeFirst / 2.0f);
	pipe2Transform.position.x = transform.position.x; pipe2Transform.position.y = (-h + sizeSecond / 2.0f);
	pipe1Transform.scale.x = pipe2Transform.scale.x = 1.5f;
	pipe1Transform.scale.y = sizeFirst; pipe2Transform.scale.y = sizeSecond;

	transform.position.x = mainCamera->GetWidth() + 1.5f;

	pipe1.GetComponent<SpriteRenderer>().color = Color::White();
	pipe2.GetComponent<SpriteRenderer>().color = Color::White();

	rigidbody1->Create();
	rigidbody2->Create();

	GameManager::instance->SetLastTop(pipe1Transform);
	GameManager::instance->SetLastBottom(pipe2Transform);
}
