#pragma once
#include <glm/mat4x4.hpp>
#include "Scenes/Entities/GameObject.h"
#include "Utils/Color.h"

namespace SoulEngine
{
	struct Transform;
	struct CameraBuffer
	{
		glm::mat4 projectionView;
	};

	struct MainCamera : public GameObject
	{
	public:
		Transform* transform;
		Color clearColor;

		MainCamera(Scene* scene, entt::entity entity);

		void SetProjection(float size, float zoom, float aspect, float zNear, float zFar);

		void GetProjection(glm::mat4& projection) const { projection = _projection; }
		void GetProjectionView(glm::mat4& projection) const;

		float GetSize() const { return _size; }
		void SetSize(float size) { _size = size; SetProjectionView(); }

		float GetAspect() const { return _aspect; }
		void SetAspect(float aspect) { _aspect = aspect; SetProjectionView(); }

		float GetZoom() const { return _zoom; }
		void SetZoom(float zoom) { _zoom = zoom; SetProjectionView(); }

		float GetZFar() const { return _zFar; }
		void SetZFar(float zFar) { _zFar = zFar; SetProjectionView(); }

		float GetZNear() const { return _zNear; }
		void SetZNear(float zNear) { _zNear = zNear; SetProjectionView(); }

		float GetHeight() const { return _height; }
		float GetWidth() const { return _width; }

	private:

		float _size{ 5.0f }, _zoom{ 1.0f }, _aspect{ 1.0f }, _width{ 1.0f }, _height{ 1.0f };
		float _zNear{ -1.0f }, _zFar{ 1.0f };

		glm::mat4 _projection{ 1.0f };
		glm::mat4 _view{ 1.0f };

		void SetProjectionView();
	};
}