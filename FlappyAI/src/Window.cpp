#include "Window.h"
#include "GameManager.h"
#include "PipeMovement.h"
#include "BirdAI.h"
#include "SoulEngine.h"

#pragma once

using namespace SoulEngine;

void Window::HandleCreated()
{
	SwapInterval(1);
	SetTitle("Flapy AI");

	auto scene = SceneManager::Create("FlapyAI");
	
	// camera settings
	auto camera = scene->GetCamera();
	camera->SetProjection(5.0f, 1.0f, static_cast<float>(GetWidth()) / static_cast<float>(GetHeight()), -1.0f, 1.0f);
	auto& gameManager = camera->AddComponent<GameManager>();
	gameManager.birdX = -10.0f;
	camera->clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	
	{
		auto top = scene->CreateGameObject("platform", 0.0f, camera->GetHeight() + 0.5f, camera->GetWidth() * 2.0f, 1.0f);
		auto& topRenderer = top.AddComponent<SpriteRenderer>();
		top.AddComponent<Rigidbody2D>();
		auto& collider1 = top.AddComponent<BoxCollider2D>();
		collider1.SetTrigger(true);
		topRenderer.color = Color::Green();

		auto bottom = scene->CreateGameObject("platform", 0.0f, -camera->GetHeight() + 0.5f, camera->GetWidth() * 2.0f, 1.0f);
		auto& bottomRenderer = bottom.AddComponent<SpriteRenderer>();
		bottom.AddComponent<Rigidbody2D>();
		auto& collider2 = bottom.AddComponent<BoxCollider2D>();
		collider2.SetTrigger(true);
		bottomRenderer.color = { 0.90f, 0.23f, 0.23f, 1.0f };
	}
	
	{
		auto first = scene->CreateGameObject("Pipe", camera->GetWidth() + 1.5f, 0.0f, 0.0f, 0.0f);
		gameManager.pipes.emplace_back(&first.AddComponent<PipeMovement>());
	
		auto second = scene->CreateGameObject("Pipe", (camera->GetWidth() + 1.5f) * 2.0f, 0.0f, 0.0f, 0.0f);
		gameManager.pipes.emplace_back(&second.AddComponent<PipeMovement>());
	}

	{
		auto gameObject = scene->CreateGameObject("BirdAI");
		gameObject.AddComponent<BirdAI>();
	}
}
