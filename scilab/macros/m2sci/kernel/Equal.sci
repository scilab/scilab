function eq=Equal(lhslist,expression)
// Copyright INRIA
// Create a new 'equal' tlist
// V.C.

rhs=argn(2)

if rhs<>2 then
  error("Equal(): wrong number of inputs!")
end
if typeof(lhslist)<>"list" then
  error("Equal(): lhslist should be a list and not a: "+typeof(lhslist))
end 
if and(typeof(expression)<>["funcall","cste","operation","variable"]) then
  disp(expression)
  error("Equal(): expression can not be a: "+typeof(expression))
end 

eq=tlist(["equal","lhs","expression"],lhslist,expression)

endfunction
