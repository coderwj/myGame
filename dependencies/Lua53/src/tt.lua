
local a = {}
a.f = function()
	if a then
		print("ffffffffffff")
	else
		print("eeeeeeeeeeeeee")
	end
end
local b = {}
b.f = a.f

b.f()

a = nil

b.f()

