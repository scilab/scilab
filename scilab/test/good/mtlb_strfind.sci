function y=mtlb_strfind(A,B)
// Copyright INRIA
// Emulation function for strfind() Matlab function
// V.C.

if type(A)==10 & type(B)==10 then
  y=strindex(A,B)
else
  if A==B then
    y=1
  else
    y=[]
  end
end
endfunction
