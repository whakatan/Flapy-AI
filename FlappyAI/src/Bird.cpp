#include "Bird.h"
#include "SoulEngine.h"

void Bird::Jump()
{
	if (rigidbody2D->GetVelocity().y <= 0)
		rigidbody2D->AddForce({ 0.0f, jumpForce }, ForceMode2D::Impulse);
}

void Bird::Initialize()
{
	min = -1.0f; max = 1.0f;
	rigidbody2D = &GetComponent<Rigidbody2D>();

	sensor->sensors.emplace_back(-65.0f, 12.0f);
	sensor->sensors.emplace_back(-35.0f, 8.0f);
	sensor->sensors.emplace_back(0.0f, 8.0f);
	sensor->sensors.emplace_back(35.0f, 8.0f);
	sensor->sensors.emplace_back(65.0f, 12.0f);

	layers.emplace_back(3);
	layers.emplace_back(1);
}

void Bird::Update()
{
	if (Input::GetKeyDown(Keys::F10))
		sensor->showSensors = !sensor->showSensors;
}

void Bird::Begin()
{
	GetComponent<CircleRenderer>().color = { Random::Next(), Random::Next(), Random::Next(), 1.0f };
}

void Bird::OnAction(const std::vector<float>& inputs)
{
	AddFitness(Time::GetDeltaTime());

	if (inputs[0] < 0.5f)
		Jump();
}

void Bird::OnTriggerEnter(const Collision2D& collision)
{
	auto tag = collision.GetGameObject().GetTag();
	if (tag == "P" || tag == "platform")
	{
		rigidbody2D->SetAwake(false);
		GetComponent<CircleRenderer>().visible = false;
		isAlive = false;
	}
}
