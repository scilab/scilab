function operation_tlist=Operation(operator,operands,out)
// Copyright INRIA
// Create a new operation tlist

rhs=argn(2)

// Verify input value
if rhs~=3 then
  error("Operation(): wrong number of rhs arguments !");
end

if typeof(operator)~="string" then
  error("Operation(): operator must be a string instead of a "+typeof(operator)); 
elseif typeof(operands)~="list" then
  error("Operation(): operands must be a list instead of a "+typeof(operands)); 
elseif typeof(out)~="list" then
  error("Operation(): out must be a list instead of a "+typeof(out));
end  

operation_tlist=tlist(["operation","operator","operands","out"],operator,operands,out)
endfunction
