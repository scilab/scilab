function var=gettempvar(n)
// Copyright INRIA
[lhs,rhs]=argn(0)
var='%v'
if level(1)>0 then
  var=var+string(level(1))
end
if level(2)>0 then
  var=var+'_'+string(level(2))
end
if rhs==1 then
  var=var+'$'+string(n)
end
