function operation_tlist=Operation(operator,operands,out)
// Copyright INRIA
// Create a new operation tlist

rhs=argn(2)

// Verify input value
if rhs~=3 then
  error(gettext("errors","m2sci_error_5"))
end

if typeof(operator)~="string" then
  error(msprintf(gettext("errors","m2sci_error_19"),typeof(operator)));; 
elseif typeof(operands)~="list" then
  error(msprintf(gettext("errors","m2sci_error_20"),typeof(operands))); 
elseif typeof(out)~="list" then
  error(msprintf(gettext("errors","m2sci_error_21"),typeof(out)));
end  

operation_tlist=tlist(["operation","operator","operands","out"],operator,operands,out)
endfunction
