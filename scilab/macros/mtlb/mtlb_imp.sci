function y=mtlb_imp(a,b,c)
// Copyright INRIA
// Emulation function for Matlab colon
// V.C.

[lhs,rhs]=argn();

if rhs==2 then
  if a==[] | b==[] then
    y=[]
  elseif isinf(a) | isnan(a) | isinf(b) | isnan(b) then
    y=%nan
  else
    y=a:b
  end
else
  if a==[] | b==[] | c==[] then
    y=[]
  elseif isinf(a) | isnan(a) | isinf(b) | isnan(b) | isinf(c) | isnan(c) then
    y=%nan
  else
    y=a:b:c
  end
end
endfunction
