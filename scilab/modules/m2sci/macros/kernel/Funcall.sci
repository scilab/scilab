function funcall_tlist=Funcall(name,lhsnb,rhslist,lhslist)
// Copyright INRIA
// Create a new function tlist

rhs=argn(2)

// Verify input value
if rhs~=4 then
  error(gettext("errors","m2sci_error_5"));
elseif typeof(name)~="string" then
  error(msprintf(gettext("errors","m2sci_error_9"),typeof(name))); 
elseif typeof(lhsnb)~="constant" then
  error(msprintf(gettext("errors","m2sci_error_10"),typeof(lhsnb))); 
elseif typeof(rhslist)~="list" & rhslist<>[] then
  error(msprintf(gettext("errors","m2sci_error_11"),typeof(rhslist))); 
elseif typeof(lhslist)~="list" then
  error(msprintf(gettext("errors","m2sci_error_12"),typeof(lhslist))); 
end  

funcall_tlist=tlist(["funcall","name","lhsnb","rhs","lhs"],name,lhsnb,rhslist,lhslist)
endfunction
