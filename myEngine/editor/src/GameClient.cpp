#include "GameClient.h"

#include <string>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES
#include <thread>
#include <sstream>
#include <assert.h>

#include "Character.h"
#include "CameraOption.h"
#include "Config.h"

//#include "luaClientPort.h"
#include "tinyxml2.h"

#include "Engine.h"
#include "HelperFunc.h"

#include "imguiContext.h"


#ifdef WIN32
#include <Windows.h>
#define sleepFunction(t) Sleep(t)
#else
#include <unistd.h>
#define sleepFunction(t) usleep(t)
#endif

using namespace std;
using namespace myEngine;

namespace myGame
{
	struct MouseState
	{
		MouseState()
			: m_mx(0)
			, m_my(0)
			, m_mz(0)
			, m_button(0)
		{
		}

		int32_t m_mx;
		int32_t m_my;
		int32_t m_mz;
		uint8_t m_button;
	};

	static MouseState s_mouseState;

	//static data.
	GameClient * GameClient::m_pGameClient = NULL;

	//static functions.
	GameClient * GameClient::getInstance()
	{
		if (m_pGameClient == NULL)
			m_pGameClient = new GameClient();
		return m_pGameClient;
	}

	void GameClient::destroyInstance()
	{
		if (m_pGameClient)
		{
			m_pGameClient->onDestroy();
			delete(m_pGameClient);
			m_pGameClient = NULL;
		}
	}

	GameClient::GameClient() :
		m_mainCharacter(nullptr),
		m_cameraOption(nullptr),
		m_state(nullptr),
		m_nowTime(0),
		m_fps(60),
		m_windowWidth(0),
		m_windowHeight(0)
	{
	}

	GameClient::~GameClient()
	{
	}

	void GameClient::onDestroy()
	{
		if (m_cameraOption)
		{
			delete(m_cameraOption);
			m_cameraOption = nullptr;
		}
		m_state = nullptr;

		for (std::pair<std::string, Character*> c : m_characters)
		{
			if (c.second)
			{
				delete(c.second);
			}
		}
		m_characters.clear();

		imguiDestroy();
	}
	
	
	bool GameClient::init(){

		//init client config.
		Config::InitConfigMap();

		//init Engine.
		Engine* pEngine = Engine::getInstance();
		assert(nullptr != pEngine);
		pEngine->init();

		imguiCreate(20);

		m_cameraOption = new CameraOption();
		m_cameraOption->setCamera(pEngine->getMaincCamera());
		m_cameraOption->resetCameraPos();

	    return true;
	}

	void GameClient::tick()
	{
		if (m_nowTime == 0)
			m_nowTime = HelperFunc::GetCurrentTimeMs();

		//begin tick, get time.
		long long new_time = HelperFunc::GetCurrentTimeMs();

		m_deltaTime = new_time - m_nowTime;
		m_nowTime = new_time;

		_tick(static_cast<int>(m_deltaTime));

		imguiBeginFrame(s_mouseState.m_mx, s_mouseState.m_my, s_mouseState.m_button, s_mouseState.m_mz, m_windowWidth, m_windowHeight);

		Engine* pEngine = Engine::getInstance();
		if (pEngine)
			pEngine->render();

		ImGui::SetNextWindowPos(ImVec2::ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2::ImVec2(400.f, 720.f), ImGuiCond_FirstUseEver);
		ImGui::Begin("Editor", NULL, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { }
				if (ImGui::MenuItem("Close", "Ctrl+W")) { }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("FPS:  %d.", static_cast<int>(1000.f / m_deltaTime + 0.5f));
		ImGui::Separator();
		if (ImGui::TreeNode("Camera Option"))
		{
			if (ImGui::Button("Reset", ImVec2(80.f, 20.f)))
			{
				if (NULL != m_cameraOption)
				{
					m_cameraOption->resetCameraPos();
				}
			}
			static float startSpeed = m_cameraOption->getMoveSpeed();
			static float currentSpeed = startSpeed;
			if (ImGui::SliderFloat("Camera Speed", &currentSpeed, 0.1f * startSpeed, 10.f * startSpeed, "Speed = %.2f"))
			{
				if (NULL != m_cameraOption)
				{
					m_cameraOption->setMoveSpeed(currentSpeed);
				}
			}
			ImGui::TreePop();
		}
		ImGui::Separator();

		ImGui::BeginGroup();
		std::vector<std::string> modelNames;
		modelNames.push_back("model_1");
		modelNames.push_back("model_2");
		modelNames.push_back("model_3");
		modelNames.push_back("model_4");
		modelNames.push_back("model_5");
		modelNames.push_back("model_6");
		modelNames.push_back("scene_1");
		static bool showModel[7] = {false, false, false, false, false, true, false};
		for (size_t i = 0; i < 7; i++)
		{
			ImGui::Checkbox(modelNames[i].c_str(), &(showModel[i]));
			if (showModel[i])
			{
				if (m_characters.find(modelNames[i]) == m_characters.end())
				{
					Character* c = Character::Create(modelNames[i].c_str());
					m_characters.insert(std::make_pair(modelNames[i], c));
				}
				m_characters[modelNames[i]]->setModelVisible(true);
			}
			else
			{
				if (m_characters.find(modelNames[i]) != m_characters.end())
				{
					m_characters[modelNames[i]]->setModelVisible(false);
				}
			}
		}
		ImGui::EndGroup();

		ImGui::Separator();
		ImGui::End();

		ImGui::End();
		ImGui::Render();

		imguiEndFrame();

		//end tick, get time.
		long long runTime = HelperFunc::GetCurrentTimeMs() - m_nowTime;
		if (runTime < static_cast<long long>(1000 / getFps()))
		{
			long long sleepTime = static_cast<long long>(1000 / getFps()) - runTime;
			sleepFunction(static_cast<unsigned long>(sleepTime));
		}
	}

	void GameClient::_tick(int delta)
	{
		//lua_tinker::call<void>(m_state, "LuaGameMgr", "Tick", delta);
		Engine* pEngine = Engine::getInstance();
		if (pEngine)
			pEngine->tick(delta);
		for (std::pair<std::string, Character*> c : m_characters)
		{
			if (c.second)
			{
				c.second->tick(delta);
			}
		}

	}

	void GameClient::handleCharInput(int key)
	{

	}

	void GameClient::handleKeyDown(int key)
	{

	}

	void GameClient::handleKeyUp(int key)
	{

	}

	void GameClient::handleMouseWheel(bool wheel_down, int scroll_delta, int x, int y)
	{
		if (nullptr != m_cameraOption)
			m_cameraOption->processMouseScroll(scroll_delta);
	}

	void GameClient::handleTouchBegin(int x, int y)
	{
		m_touchBeginPosX = x;
		m_touchBeginPosY = y;
		m_touchPosX = x;
		m_touchPosY = y;
		s_mouseState.m_mx = m_touchPosX;
		s_mouseState.m_my = m_touchPosY;
		s_mouseState.m_button = 1;
	}

	void GameClient::handleTouchMove(int x, int y)
	{
		if (nullptr == m_cameraOption)
			return;
		if (ImGui::GetIO().WantCaptureMouse)
		{
			m_touchPosX = x;
			m_touchPosY = y;
			s_mouseState.m_mx = m_touchPosX;
			s_mouseState.m_my = m_touchPosY;
			return;
		}
		int dx = x - m_touchPosX;
		m_cameraOption->processKeyboard(CameraMoveDir::ROTATERIGHT, dx);
		int dy = y - m_touchPosY;
		m_cameraOption->processKeyboard(CameraMoveDir::ROTATEDOWN, dy);
		m_touchPosX = x;
		m_touchPosY = y;
		s_mouseState.m_mx = m_touchPosX;
		s_mouseState.m_my = m_touchPosY;
	}

	void GameClient::handleTouchEnd(int x, int y)
	{
		m_touchPosX = x;
		m_touchPosY = y;
		s_mouseState.m_mx = m_touchPosX;
		s_mouseState.m_my = m_touchPosY;
		s_mouseState.m_button = 0;
	}

	void GameClient::onResize(int width, int height)
	{
		if (m_windowWidth == width && m_windowHeight == height)
			return;
		m_windowWidth = width;
		m_windowHeight = height;
		Engine* pEngine = Engine::getInstance();
		if (nullptr != pEngine)
		{
			pEngine->onResize(width, height);
		}
	}

	int GameClient::getFps() const
	{
		return m_fps;
	}

	void GameClient::setFps(int val)
	{
		m_fps = val;
	}
	

}