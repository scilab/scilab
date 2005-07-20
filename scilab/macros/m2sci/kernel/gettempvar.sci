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
if size(level,1)>1 then
  if level(size(level,1))==0 then
    varname=varname+string(size(level,1))
  elseif level(size(level,1))>0 then   // If in statements of a clause then variable name is modified by adding _level
    varname=varname+"_"+string(level(size(level,1)))
  end
end

// User is allowed to precise a variable number
if rhs==1 then
  varname=varname+"$"+string(n)
end

var=Variable(varname,Infer(list(0,0),Type()))
endfunction
