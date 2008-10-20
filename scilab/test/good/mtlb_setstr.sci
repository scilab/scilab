function y=mtlb_setstr(x)
// Copyright INRIA
// Emulation function for setstr() Matlab function
// V.C.

if type(x)==10 then
  y=x
else
  if or(type(x)==[4,6]) then
    x=bool2s(x)
  end
  y=[]
  for k=1:size(x,1)
    y=[y;ascii(x(k,:))]
  end
  // When x is an empty matrix
  if isempty(y) then
    y=""
  end
end
endfunction
