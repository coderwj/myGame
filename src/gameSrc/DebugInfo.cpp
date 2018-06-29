#include "DebugInfo.h"
#include "GameScene.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw_gl3.h"

using namespace ImGui;

DebugInfo* DebugInfo::debugInfo = NULL;

DebugInfo::DebugInfo()
:m_deltaTime(1.f)
{

}

bool DebugInfo::init()
{
	return true;
}

void DebugInfo::tick(float delta)
{
	m_deltaTime = delta;
}

void DebugInfo::render()
{
	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Text("FPS:  %.3f.", 1.f / m_deltaTime);
	if (ImGui::ButtonEx("Reset Camera", ImVec2(100.f, 20.f), ImGuiButtonFlags_PressedOnClick))
	{
		GameScene* pGameScene = GameScene::getInstance();
		if (pGameScene)
		{
			pGameScene->resetCameraPos();
		}
	}
	ImGui::Render();
}
