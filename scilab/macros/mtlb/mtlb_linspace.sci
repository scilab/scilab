function y=mtlb_linspace(a,b,n)
// Copyright INRIA
// Emulation function for linspace() Matlab function
// V.C.
str=%F
if type(a)==10 then
  a=asciimat(a)
  str=%T
end
if type(b)==10 then
  b=asciimat(b)
  str=%T
end

if or(type(a)==[4,6]) then
  a=bool2s(a)
end
if or(type(b)==[4,6]) then
  b=bool2s(b)
end

[lhs,rhs]=argn(0)

if rhs==2 then
  if str then
    y=ascii(linspace(a,b))
  else
    y=linspace(a,b)
  end
else
  if type(n)==10 then
    n=asciimat(n)
  end
  if or(type(n)==[4,6]) then
    n=bool2s(n)
  end
  if str then
    y=ascii(linspace(a,b,n))
  else
    y=linspace(a,b,n)
  end
end
endfunction
