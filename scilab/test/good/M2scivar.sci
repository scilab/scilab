function var=M2scivar(sciname,matname,infer,call_level)
// Copyright INRIA
// Create a new 'm2scivar' tlist

rhs=argn(2)

if rhs<3 then
  error("M2scivar(): wrong number of rhs arguments !")
end
if rhs==3 then
  call_level=0
end

if typeof(sciname)~="string" then
  error("M2scivar(): sciname must be a string instead of a: "+typeof(sciname))
elseif typeof(matname)~="string" then
  error("M2scivar(): matname must be a string instead of a: "+typeof(matname))
elseif typeof(infer)~="infer" then
  error("M2scivar(): infer must be an ''infer'' tlist instead of a: "+typeof(infer))
end

var=tlist(["m2scivar","sciname","matname","infer","level"],sciname,matname,infer,call_level)

endfunction
