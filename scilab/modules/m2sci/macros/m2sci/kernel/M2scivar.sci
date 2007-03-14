function var=M2scivar(sciname,matname,infer,call_level)
// Copyright INRIA
// Create a new 'm2scivar' tlist

rhs=argn(2)

if rhs<3 then
  error(gettext("errors","m2sci_error_5"))
end
if rhs==3 then
  call_level=0
end

if typeof(sciname)~="string" then
  error(msprintf(gettext("errors","m2sci_error_16"),typeof(sciname)));
elseif typeof(matname)~="string" then
  error(msprintf(gettext("errors","m2sci_error_17"),typeof(matname)));
elseif typeof(infer)~="infer" then
  error(msprintf(gettext("errors","m2sci_error_18"),typeof(infer)));
end

var=tlist(["m2scivar","sciname","matname","infer","level"],sciname,matname,infer,call_level)

endfunction
