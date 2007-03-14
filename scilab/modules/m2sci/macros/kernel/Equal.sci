function eq=Equal(lhslist,expression,endsymbol)
// Copyright INRIA
// Create a new 'equal' tlist
// V.C.

rhs=argn(2)

if rhs<2 then
  error(gettext("errors","m2sci_error_5"))
end
if rhs<3 then
  endsymbol=";"
end
if typeof(lhslist)<>"list" then
  error(msprintf(gettext("errors","m2sci_error_6"),typeof(lhslist)));
end 
if and(typeof(expression)<>["funcall","cste","operation","variable"]) then
  disp(expression)
  error(msprintf(gettext("errors","m2sci_error_7"),typeof(expression)))
end 
if type(endsymbol)<>10 then
  error(msprintf(gettext("errors","m2sci_error_8"),typeof(lhslist)));
end 

eq=tlist(["equal","lhs","expression","endsymbol"],lhslist,expression,endsymbol)

endfunction
