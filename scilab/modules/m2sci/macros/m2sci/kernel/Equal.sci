function eq=Equal(lhslist,expression,endsymbol)
// Copyright INRIA
// Create a new 'equal' tlist
// V.C.

rhs=argn(2)

if rhs<2 then
  error("Equal(): wrong number of inputs!")
end
if rhs<3 then
  endsymbol=";"
end
if typeof(lhslist)<>"list" then
  error("Equal(): lhslist should be a list and not a: "+typeof(lhslist))
end 
if and(typeof(expression)<>["funcall","cste","operation","variable"]) then
  disp(expression)
  error("Equal(): expression can not be a: "+typeof(expression))
end 
if type(endsymbol)<>10 then
  error("Equal(): endsymbol should be a character string and not a: "+typeof(lhslist))
end 

eq=tlist(["equal","lhs","expression","endsymbol"],lhslist,expression,endsymbol)

endfunction
