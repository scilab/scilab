function [boolval,index]=isdefinedvar(var)
// Copyright INRIA
// Verify if variable is defined in varslist
// Input:
// - var: a M2SCI tlist (funcall, operation...) or a character string (name) for special variables
// Special variables i, j, pi, eps, nargin, nargout
// Output:
// - boolval:
//   %T if input tlist is a defined 'variable' (already found while converting M-file)
//   %F if input is not a 'variable' or an uknown 'variable'
// V.C.

// Global variable for M2SCI
global("varslist")

// level is declared in m2sci.sci and modified in clause2sci.sci
level;

boolval=%F
index=[]

// If input is a string
if type(var)==10 then
  if and(var<>["i","j","pi","eps","%nargin","%nargout"]) then
    warning("Bad use of isdefinedvar() with input: "+var)
    return
  end
  varname=var
else
  // If input is not a 'variable', it can be a defined variable...
  if typeof(var)=="variable" then
    varname=var.name
  elseif typeof(var)=="m2scivar" then
    varname=var.matname
  else
    return
  end
end

// Compare with all existing variables in varslist
for k=1:size(varslist)
  if varslist(k).matname==varname then
    if level(1)<=0 | varslist(k).level<=level(1) then
      boolval=%T
      index=k
      return
    end
  end
end
endfunction
