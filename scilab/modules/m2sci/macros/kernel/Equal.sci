function eq=Equal(lhslist,expression,endsymbol)
// Copyright INRIA
// Create a new 'equal' tlist
// V.C.

rhs=argn(2)

if rhs<2 then
  error(gettext("Wrong number of inputs."))
end
if rhs<3 then
  endsymbol=";"
end
if typeof(lhslist)<>"list" then
  error(msprintf(gettext("lhslist should be a list and not a: %s."),typeof(lhslist)));
end 
if and(typeof(expression)<>["funcall","cste","operation","variable"]) then
  disp(expression)
  error(msprintf(gettext("expression can not be a: %s."),typeof(expression)))
end 
if type(endsymbol)<>10 then
  error(msprintf(gettext("endsymbol should be a character string and not a: %s."),typeof(lhslist)));
end 

eq=tlist(["equal","lhs","expression","endsymbol"],lhslist,expression,endsymbol)

endfunction
