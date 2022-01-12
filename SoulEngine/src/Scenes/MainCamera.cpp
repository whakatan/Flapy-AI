#include "pch.h"
#include "Scenes/MainCamera.h"
#include "Scenes/Entities/Components.h"

#include <glm/gtc/matrix_transform.hpp>

namespace SoulEngine
{
	MainCamera::MainCamera(Scene* scene, entt::entity entity) : GameObject(scene, entity), clearColor(Color::Black())
	{
		transform = &AddComponent<Transform>();
		transform->position.z = 1.0f;
	}
	
	void MainCamera::SetProjection(float size, float zoom, float aspect, float zNear, float zFar)
	{
		_size = size;
		_zoom = zoom;
		_aspect = aspect;
		_zNear = zNear;
		_zFar = zFar;
		SetProjectionView();
	}

	void MainCamera::GetProjectionView(glm::mat4& projection) const
	{
		projection = _projection * glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 1.0, 0.0), glm::vec4(transform->position, 1.0));
	}

	void MainCamera::SetProjectionView()
	{
		_height = _size * (1.0f / _zoom);
		_width = _aspect * _height;
		_projection = glm::ortho(-_width, _width, -_height, _height, _zNear, _zFar);
	}
}