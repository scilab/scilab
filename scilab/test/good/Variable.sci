function var=Variable(name,infer)
// Copyright INRIA
// Create a new variable tlist

rhs=argn(2)

if rhs<>2 then
  error("Variable(): wrong number if inputs !")
end

if typeof(name)~="string" then
  error("Variable(): name must be a string instead of a: "+typeof(name))
elseif typeof(infer)~="infer" then
  error("Variable(): infer must be an ''infer'' tlist instead of a: "+typeof(infer))
end

var=tlist(["variable","name","infer"],name,infer)

endfunction
