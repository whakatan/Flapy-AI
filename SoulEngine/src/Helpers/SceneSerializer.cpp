#include "pch.h"
#include "Helpers/SceneSerializer.h"
#include "Scenes/Scene.h"
#include "Scenes/MainCamera.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Entities/GameObject.h"
#include "Scenes/Entities/Components.h"
#include "Scenes/Entities/Rigidbody2D.h"
#include "Scenes/Entities/Colliders.h"
#include "Rendering/Texture.h"
#include "Assets/Textures/TextureManager.h"
#include "Assets/Fonts/FontManager.h"
#include "Assets/Fonts/Font.h"

#include <entt.hpp>
#include <json.hpp>

namespace SoulEngine
{
	void SerializeTag(nlohmann::json& json, GameObject& gameObject);
	void SerializeTransform(nlohmann::json& json, GameObject& gameObject);
	void SerializeSpriteRenderer(nlohmann::json& json, GameObject& gameObject, const std::string& path);
	void SerializeCircleRenderer(nlohmann::json& json, GameObject& gameObject);
	void SerializeTextRenderer(nlohmann::json& json, GameObject& gameObject, const std::string& path);
	void SerializeRigidbody2D(nlohmann::json& json, GameObject& gameObject);
	void SerializeBoxCollider2D(nlohmann::json& json, GameObject& gameObject);
	void SerializeCircleCollider2D(nlohmann::json& json, GameObject& gameObject);

	void SceneSerializer::Serialize(Scene* scene)
	{
		if (!scene) return;

		nlohmann::json j;
		nlohmann::json entities;

		std::string path;
		GetExecutableDirectory(path);

		scene->_registry.each([&](auto e){

			GameObject gameObject{ scene, e };
			if (!gameObject.HasComponent<IgnoreSerialize>() && gameObject.HasComponent<Id>())
			{
				nlohmann::json components;

				SerializeTag(components, gameObject);
				SerializeTransform(components, gameObject);
				SerializeSpriteRenderer(components, gameObject, path);
				SerializeCircleRenderer(components, gameObject);
				SerializeTextRenderer(components, gameObject, path);
				SerializeRigidbody2D(components, gameObject);
				SerializeBoxCollider2D(components, gameObject);
				SerializeCircleCollider2D(components, gameObject);

				components["GameObject"] = static_cast<uint64_t>(gameObject.GetId());

				entities.push_back(components);
			}

			});

		auto camera = scene->GetCamera();

		j["Scene"] = {
			{ "Name", scene->_name },
			{ "MainCamera", {
				{ "Size", camera->GetSize() },
				{ "Zoom", camera->GetZoom() },
				{ "Aspect", camera->GetAspect() },
				{ "Near", camera->GetZNear() },
				{ "Far", camera->GetZFar() } ,
				{ "ClearColor", { { "r", camera->clearColor.R() }, { "g", camera->clearColor.G() }, { "b", camera->clearColor.B() }, { "a", camera->clearColor.A() } } },
			}},
			{ "GameObjects", entities }
		};

		path = path + "Assets" + PATH_SEPARATOR + "Scenes";
		if (!std::filesystem::is_directory(path))
			std::filesystem::create_directory(path);

		std::ofstream o(path + PATH_SEPARATOR + scene->_name + ".json");
		o << std::setw(4) << j << std::endl;
	}

	Scene* SceneSerializer::Deserialize(const std::string& name)
	{
		std::string path;
		std::string executableDirectory;
		GetExecutableDirectory(executableDirectory);
		path = executableDirectory + "Assets" + PATH_SEPARATOR + "Scenes";
		path += PATH_SEPARATOR + std::string(name) + ".json";
		if (!std::filesystem::exists(path))
			return nullptr;

		std::ifstream stream(path);

		nlohmann::json j;
		stream >> j;
		
		if (!j.contains("Scene")) return nullptr;

		auto& sceneSource = j["Scene"];
		Scene* scene = SceneManager::Create(sceneSource["Name"]);
		// camera
		auto& cameraSource = sceneSource["MainCamera"];
		auto camera = scene->GetCamera();
		camera->SetProjection(cameraSource["Size"], cameraSource["Zoom"], cameraSource["Aspect"], cameraSource["Near"], cameraSource["Far"]);
		camera->clearColor = { cameraSource["ClearColor"]["r"], cameraSource["ClearColor"]["g"], cameraSource["ClearColor"]["b"], cameraSource["ClearColor"]["a"] };

		if (!sceneSource.contains("GameObjects")) return scene;

		auto& entities = sceneSource["GameObjects"];
		for (auto& entitiy : entities)
		{
			auto& tagSource = entitiy["Tag"];
			auto& transformSource = entitiy["Transform"];

			auto gameObject = scene->CreateGameObject(tagSource);

			auto& id = gameObject.GetComponent<Id>();
			id.uuid = static_cast<uint64_t>(entitiy["GameObject"]);

			// Transform
			auto& transform = gameObject.GetTransform();
			transform.position.x = transformSource["position"]["x"];
			transform.position.y = transformSource["position"]["y"];
			transform.position.z = transformSource["position"]["z"];
			transform.scale.x = transformSource["scale"]["x"];
			transform.scale.y = transformSource["scale"]["y"];
			transform.scale.z = transformSource["scale"]["z"];
			transform.rotation.x = transformSource["rotation"]["x"];
			transform.rotation.y = transformSource["rotation"]["y"];
			transform.rotation.z = transformSource["rotation"]["z"];

			// SpriteRenderer
			if (entitiy.contains("SpriteRenderer"))
			{
				auto& rendererSource = entitiy["SpriteRenderer"];
				auto& color = rendererSource["color"];
				auto& textureRect = rendererSource["textureRect"];

				std::string assetpath;
				rendererSource["texturePath"].get_to(assetpath);

				auto& renderer = gameObject.AddComponent<SpriteRenderer>();

				renderer.color = { color["r"], color["g"], color["b"], color["a"] };
				if (rendererSource.contains("texturePath") && rendererSource["texturePath"] != "")
					renderer.texture = TextureManager::Load(executableDirectory + assetpath);
				renderer.textureRect = { textureRect["x"], textureRect["y"], textureRect["z"], textureRect["w"] };
			}
			// CircleRenderer
			else if (entitiy.contains("CircleRenderer"))
			{
				auto& rendererSource = entitiy["CircleRenderer"];
				auto& color = rendererSource["color"];

				auto& renderer = gameObject.AddComponent<CircleRenderer>();

				renderer.color = { color["r"], color["g"], color["b"], color["a"] };
				renderer.thickness = rendererSource["thickness"];
			}
			// TextRenderer
			else if (entitiy.contains("TextRenderer"))
			{
				auto& rendererSource = entitiy["TextRenderer"];
				auto& color = rendererSource["color"];
				auto& font = rendererSource["font"];

				std::string assetpath;
				font["path"].get_to(assetpath);

				auto& renderer = gameObject.AddComponent<TextRenderer>();

				renderer.color = { color["r"], color["g"], color["b"], color["a"] };
				renderer.font = FontManager::Load(executableDirectory + assetpath, font["size"]);
				renderer.text = rendererSource["text"];
			}
			// Rigidbody2D
			if (entitiy.contains("Rigidbody2D"))
			{
				auto& rigidbodySource = entitiy["Rigidbody2D"];
				auto& rigidbody = gameObject.AddComponent<Rigidbody2D>();

				rigidbody.SetEnabled(rigidbodySource["enabled"]);
				rigidbody.SetAwake(rigidbodySource["awake"]);
				rigidbody.SetBullet(rigidbodySource["bullet"]);
				rigidbody.SetGravityScale(rigidbodySource["gravityScale"]);
				rigidbody.SetFixedRotation(rigidbodySource["fixedRotation"]);
				rigidbody.SetBodyType(rigidbodySource["bodyType"]);
			}
			// BoxCollider2D
			if (entitiy.contains("BoxCollider2D"))
			{
				auto& colliderSource = entitiy["BoxCollider2D"];
				auto& collider = gameObject.AddComponent<BoxCollider2D>();

				collider.SetTrigger(colliderSource["trigger"]);
				collider.SetDensity(colliderSource["density"]);
				collider.SetFriction(colliderSource["friction"]);
				collider.SetRestitution(colliderSource["restitution"]);
				collider.SetRestitutionThreshold(colliderSource["threshold"]);
				collider.SetOffset({ colliderSource["offset"]["x"], colliderSource["offset"]["y"] });
				collider.SetSize({ colliderSource["size"]["x"], colliderSource["size"]["y"] });
			}
			// CircleCollider2D
			else if (entitiy.contains("CircleCollider2D"))
			{
				auto& colliderSource = entitiy["CircleCollider2D"];
				auto& collider = gameObject.AddComponent<CircleCollider2D>();

				collider.SetTrigger(colliderSource["trigger"]);
				collider.SetDensity(colliderSource["density"]);
				collider.SetFriction(colliderSource["friction"]);
				collider.SetRestitution(colliderSource["restitution"]);
				collider.SetRestitutionThreshold(colliderSource["threshold"]);
				collider.SetOffset({ colliderSource["offset"]["x"], colliderSource["offset"]["y"] });
				collider.SetRadius(colliderSource["radius"]);
			}
		}

		return scene;
	}

	void SerializeTag(nlohmann::json& json, GameObject& gameObject)
	{
		if (gameObject.HasComponent<Tag>())
			json["Tag"] = gameObject.GetTag();
	}
	
	void SerializeTransform(nlohmann::json& json, GameObject& gameObject)
	{
		if (gameObject.HasComponent<Transform>())
		{
			auto& transform = gameObject.GetTransform();
			json["Transform"] = {
				{ "position", { { "x", transform.position.x }, { "y", transform.position.y }, { "z", transform.position.z } } },
				{ "scale", { { "x", transform.scale.x }, { "y", transform.scale.y }, { "z", transform.scale.z } } },
				{ "rotation", { { "x", transform.rotation.x }, { "y", transform.rotation.y }, { "z", transform.rotation.z } } }
			};
		}
	}

	void SerializeSpriteRenderer(nlohmann::json& json, GameObject& gameObject, const std::string& path)
	{
		if (gameObject.HasComponent<SpriteRenderer>())
		{
			auto& renderer = gameObject.GetComponent<SpriteRenderer>();
			json["SpriteRenderer"] = {
				{ "color", { { "r", renderer.color.R() }, { "g", renderer.color.G() }, { "b", renderer.color.B() }, { "a", renderer.color.A() }  } },
				{ "texturePath", renderer.texture ? renderer.texture->GetPath() : "" },
				{ "textureRect", { { "x", renderer.textureRect.x }, { "y", renderer.textureRect.y }, { "z", renderer.textureRect.z }, { "w", renderer.textureRect.w } } }
			};
		}
	}

	void SerializeCircleRenderer(nlohmann::json& json, GameObject& gameObject)
	{
		if (gameObject.HasComponent<CircleRenderer>())
		{
			auto& renderer = gameObject.GetComponent<CircleRenderer>();
			json["CircleRenderer"] = {
				{ "color", { { "r", renderer.color.R() }, { "g", renderer.color.G() }, { "b", renderer.color.B() }, { "a", renderer.color.A() }  } },
				{ "thickness", renderer.thickness }
			};
		}
	}

	void SerializeTextRenderer(nlohmann::json& json, GameObject& gameObject, const std::string& path)
	{
		if (gameObject.HasComponent<TextRenderer>())
		{
			auto& renderer = gameObject.GetComponent<TextRenderer>();

			std::string fontPath;
			if (renderer.font)
				fontPath = renderer.font->GetPath().replace(0, path.size(), "");

			json["TextRenderer"] = {
				{ "font", { { "path", fontPath }, {"size", renderer.font->GetSize()}}},
				{ "color", { { "r", renderer.color.R() }, { "g", renderer.color.G() }, { "b", renderer.color.B() }, { "a", renderer.color.A() } } },
				{ "text", renderer.text }
			};
		}
	}

	void SerializeRigidbody2D(nlohmann::json& json, GameObject& gameObject)
	{
		if (gameObject.HasComponent<Rigidbody2D>())
		{
			auto& rigidbody2D = gameObject.GetComponent<Rigidbody2D>();
			json["Rigidbody2D"] = {
				{ "enabled", rigidbody2D.IsEnabled() },
				{ "awake", rigidbody2D.IsAwake() },
				{ "bullet", rigidbody2D.IsBullet() },
				{ "gravityScale", rigidbody2D.GetGravityScale() },
				{ "fixedRotation", rigidbody2D.IsFixedRotation() },
				{ "bodyType", rigidbody2D.GetBodyType() },
			};
		}
	}

	void SerializeBoxCollider2D(nlohmann::json& json, GameObject& gameObject)
	{
		if (gameObject.HasComponent<BoxCollider2D>())
		{
			auto& collider = gameObject.GetComponent<BoxCollider2D>();
			json["BoxCollider2D"] = {
				{ "trigger", collider.IsTrigger() },
				{ "density", collider.GetDensity() },
				{ "friction", collider.GetFriction() },
				{ "restitution", collider.GetRestitution() },
				{ "threshold", collider.GetRestitutionThreshold() },
				{ "offset", { { "x", collider.GetOffset().x }, { "y", collider.GetOffset().y }}},
				{ "size", { { "x", collider.GetSize().x }, { "y", collider.GetSize().y }}},
			};
		}
	}

	void SerializeCircleCollider2D(nlohmann::json& json, GameObject& gameObject)
	{
		if (gameObject.HasComponent<CircleCollider2D>())
		{
			auto& collider = gameObject.GetComponent<CircleCollider2D>();
			json["CircleCollider2D"] = {
				{ "trigger", collider.IsTrigger() },
				{ "density", collider.GetDensity() },
				{ "friction", collider.GetFriction() },
				{ "restitution", collider.GetRestitution() },
				{ "threshold", collider.GetRestitutionThreshold() },
				{ "offset", { { "x", collider.GetOffset().x }, { "y", collider.GetOffset().y }}},
				{ "radius", collider.GetRadius() },
			};
		}
	}
}