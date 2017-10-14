a = {}
a[1] = {"abcd"}
a[2] = {}
a[3] = {}
for k, v in pairs(a) do
	print(k.."----------------"..tostring(v))
end
