LuaUiMgr = {}

LuaUiMgr.m_time = 0

function InitGame()
    print("----LuaUiMgr.InitGame----")
end

function LuaUiMgr.InitGame()
    print("----LuaUiMgr.InitGame----")
end

function LuaUiMgr.Tick(delta)
    LuaUiMgr.m_time = LuaUiMgr.m_time + delta
    if LuaUiMgr.m_time > 10 then
    	LuaUiMgr.reloadScene()
    end
    --print("----LuaUiMgr.Tick----".. LuaUiMgr.m_time)
end

function LuaUiMgr.reloadScene()
	if LuaUiMgr.m_hasReload then
		return
	end
	LuaUiMgr.m_hasReload = true
	local pGameScene = GameScene:getInstance()
	if pGameScene then
		pGameScene:changeScene("scene_3")
	else
		print("ERROR:---- LuaUiMgr.reloadScene -----")
	end
end

return LuaUiMgr