function var=gettempvar(n)
// Copyright INRIA
// M2SCI function
// Create a temporary variable with name depending on clause level...
// V.C.
global("tmpvarnb")
[lhs,rhs]=argn(0)

varname="%v"+string(tmpvarnb)
tmpvarnb=tmpvarnb+1

// If in the expression of a clause then variable name is modified by adding level
if level(1)>0 then
  varname=varname+string(level(1))
end

// If in statements of a clause then variable name is modified by adding _level
if level(2)>0 then
  varname=varname+"_"+string(level(2))
end

// User is allowed to precise a variable number
if rhs==1 then
  varname=varname+"$"+string(n)
end

var=Variable(varname,Infer(list(0,0),Type()))
endfunction
