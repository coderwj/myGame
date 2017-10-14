local c = {}
function c.func1()
	print("bbbbbbbbbbb")
end
local b = {}
b.__index = function() return function() print("xxxxxxxxxxxxxxxx") end end 

local a = {}
setmetatable(a, b)
--a.__index = a
a.func1()
print(tostring(a.func1))