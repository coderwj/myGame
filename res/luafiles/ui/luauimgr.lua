LuaGameMgr = {}

LuaGameMgr.m_time = 0

function LuaGameMgr.InitGame()
    print("----LuaGameMgr.InitGame----")
    LuaUiMgr.InitGame()
end

function LuaGameMgr.Tick(delta)
    LuaGameMgr.m_time = LuaGameMgr.m_time + delta
    print("----LuaGameMgr.Tick----".. LuaGameMgr.m_time)
    LuaUiMgr.Tick(delta)
end

return LuaGameMgr