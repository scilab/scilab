function v=isdef(variablename,wh)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==2 then
  v=exists(variablename,wh)==1
else
  v=exists(variablename)==1
end
endfunction
