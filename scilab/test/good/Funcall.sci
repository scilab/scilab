function funcall_tlist=Funcall(name,lhsnb,rhslist,lhslist)
// Copyright INRIA
// Create a new function tlist

rhs=argn(2)

// Verify input value
if rhs~=4 then
  error("Funcall: wrong number of rhs arguments !");
elseif typeof(name)~="string" then
  error("Funcall: name must be a string instead of a "+typeof(name)); 
elseif typeof(lhsnb)~="constant" then
  error("Funcall: lhsnb must be a constant instead of a "+typeof(lhsnb)); 
elseif typeof(rhslist)~="list" & rhslist<>[] then
  error("Funcall: rhslist must be a list instead of a "+typeof(rhslist)); 
elseif typeof(lhslist)~="list" then
  error("Funcall: lhslist must be a list instead of a "+typeof(lhslist)); 
end  

funcall_tlist=tlist(["funcall","name","lhsnb","rhs","lhs"],name,lhsnb,rhslist,lhslist)
endfunction
