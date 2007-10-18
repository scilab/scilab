function operation_tlist=Operation(operator,operands,out)
// Copyright INRIA
// Create a new operation tlist

rhs=argn(2)

// Verify input value
if rhs~=3 then
  error(gettext("Wrong number of inputs."))
end

if typeof(operator)~="string" then
  error(msprintf(gettext("operator must be a string instead of a: %s."),typeof(operator)));; 
elseif typeof(operands)~="list" then
  error(msprintf(gettext("operands must be a list instead of a: %s."),typeof(operands))); 
elseif typeof(out)~="list" then
  error(msprintf(gettext("out must be a list instead of a: %s."),typeof(out)));
end  

operation_tlist=tlist(["operation","operator","operands","out"],operator,operands,out)
endfunction
