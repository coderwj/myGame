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
    --print("----LuaUiMgr.Tick----".. LuaUiMgr.m_time)
end

return LuaUiMgr