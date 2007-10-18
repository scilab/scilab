function var=Variable(name,infer)
// Copyright INRIA
// Create a new variable tlist

rhs=argn(2)

if rhs<>2 then
  error(gettext("Wrong number of inputs."))
end

if typeof(name)~="string" then
  error(msprintf(gettext("name must be a string instead of a: %s."),typeof(name)))
elseif typeof(infer)~="infer" then
  error(msprintf(gettext("infer must be an ''infer'' tlist instead of a: %s."),typeof(infer)))
end

var=tlist(["variable","name","infer"],name,infer)

endfunction
