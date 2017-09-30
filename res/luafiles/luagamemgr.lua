LuaGameMgr = {}

LuaGameMgr.m_time = 0

function InitGame()
    print("----LuaGameMgr.InitGame----")
end

function LuaGameMgr.InitGame()
    print("----LuaGameMgr.InitGame----")
end

function LuaGameMgr.Tick(delta)
    LuaGameMgr.m_time = LuaGameMgr.m_time + delta
    print("----LuaGameMgr.Tick----".. LuaGameMgr.m_time)
end

return LuaGameMgr