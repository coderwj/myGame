#pragma once

#include "MyEngineCore.h"

class DebugInfo
{
private:
	static DebugInfo * debugInfo;

	float m_deltaTime;

	float m_cameraSpeed;

public:
	static DebugInfo * getInstance() {
		if (debugInfo == NULL)
			debugInfo = new DebugInfo();
		return debugInfo;
	}

	void destroyInstance() {
		if (debugInfo)
		{
			delete(debugInfo);
			debugInfo = NULL;
		}
	}
	DebugInfo();
	~DebugInfo() {}

	bool init();
	void tick(float delta);
	void render();
};
