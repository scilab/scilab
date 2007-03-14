function var=Variable(name,infer)
// Copyright INRIA
// Create a new variable tlist

rhs=argn(2)

if rhs<>2 then
  error(gettext("errors","m2sci_error_5"))
end

if typeof(name)~="string" then
  error(msprintf(gettext("errors","m2sci_error_9"),typeof(name)))
elseif typeof(infer)~="infer" then
  error(msprintf(gettext("errors","m2sci_error_18"),typeof(infer)))
end

var=tlist(["variable","name","infer"],name,infer)

endfunction
