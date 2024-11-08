#pragma once
#include "vec.h"
#include "vec2.h"
#include <string>
#include "il2cpp_resolver.hpp"
#include "Lists.hpp"
#include <intrin.h>

namespace Functions
{
#pragma region Hooks
	void(UNITY_CALLING_CONVENTION SetFOV)(Unity::CCamera*, float);
	void SetFOV_hk(Unity::CCamera* camera, float fov)
	{
		if (vars::fov_changer)
		{
			fov = vars::CameraFOV;
		}

		return SetFOV(camera, fov);
	}
#pragma endregion

#pragma region Game Functions
	// UnityEngine_Camera$$Get_Main
	Unity::CCamera* GetMainCamera()
	{
		Unity::CCamera* (UNITY_CALLING_CONVENTION get_main)();
		return reinterpret_cast<decltype(get_main)>(sdk::GameAssembly + Offsets::MainCamera)();
	}

	// UnityEngine_Camera$$Set_FieldOfView
	void SetFieldOfView(Unity::CCamera* camera, float fov)
	{
		void (UNITY_CALLING_CONVENTION set_fieldOfView)(Unity::CCamera*, float);
		return reinterpret_cast<decltype(set_fieldOfView)>(sdk::GameAssembly + Offsets::FieldOfView)(camera, fov);
	}

#pragma endregion

#pragma region Custom Functions
	bool worldtoscreen(Unity::Vector3 world, Vector2& screen)
	{
		auto CameraObj = Unity::GameObject::Find("Main Camera"); // Find the Camera Object
		if (!CameraObj)
			return false;

		Unity::CCamera* CameraMain = (Unity::CCamera*)CameraObj->GetComponent("Camera"); // Get The Main Camera

		if (!CameraMain)
			return false;

		//Unity::Vector3 buffer = WorldToScreenPoint(CameraMain, world, 2);

		Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>("WorldToScreenPoint", world, Unity::eye::mono); // Call the worldtoscren function using monoeye (2)

		if (buffer.x > vars::screen_size.x || buffer.y > vars::screen_size.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) // check if point is on screen
		{
			return false;
		}

		if (buffer.z > 0.0f) // Check if point is in view
		{
			screen = Vector2(buffer.x, vars::screen_size.y - buffer.y);
		}

		if (screen.x > 0 || screen.y > 0) // Check if point is in view
		{
			return true;
		}
	}
#pragma endregion
}