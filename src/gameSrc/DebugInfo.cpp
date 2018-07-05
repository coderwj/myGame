#include "DebugInfo.h"
#include "Engine.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw_gl3.h"

using namespace ImGui;
using namespace myEngine;

namespace myGame
{
	DebugInfo* DebugInfo::debugInfo = NULL;
	
	DebugInfo::DebugInfo()
	:m_deltaTime(1.f)
	,m_cameraSpeed(0.f)
	{
	
	}
	
	bool DebugInfo::init()
	{
		Engine* pEngine = Engine::getInstance();
		if (NULL != pEngine)
		{
			Camera* pCamera = pEngine->getMaincCamera();
			if (NULL != pCamera)
			{
				m_cameraSpeed = pCamera->;
			}
		}
		return true;
	}
	
	void DebugInfo::tick(float delta)
	{
		m_deltaTime = delta;
		GameScene* pGameScene = GameScene::getInstance();
		if (NULL != pGameScene)
		{
			Camera* pCamera = pGameScene->getCamera();
			if (NULL != pCamera)
			{
				pCamera->MovementSpeed = m_cameraSpeed;
			}
		}
	}
	
	void DebugInfo::render()
	{
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Text("FPS:  %d.", static_cast<int>(1.f / m_deltaTime + 0.5f));
		if (ImGui::ButtonEx("Reset Camera", ImVec2(100.f, 20.f), ImGuiButtonFlags_PressedOnClick))
		{
			GameScene* pGameScene = GameScene::getInstance();
			if (pGameScene)
			{
				pGameScene->resetCameraPos();
			}
		}
		static float start = m_cameraSpeed;
		ImGui::SliderFloat("CameraMove Speed", &m_cameraSpeed, start, 8.f * start, "Speed = %.3f");
		ImGui::Render();
	}
}
