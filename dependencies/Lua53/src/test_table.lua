local a = {}
a.p = {"sb"}
a.q = a
a.r = {"sbb"}
print(tostring(a.p))
print(tostring(a.q.p))


print(tostring(a.r))
print(tostring(a.q.r))
