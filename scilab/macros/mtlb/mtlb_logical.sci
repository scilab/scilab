function K=mtlb_logical(A)
// Copyright INRIA
// Emulation function for Matlab logical()
// V.C.

if isempty(A) then
  K=[]
elseif or(type(A)==[4,6]) then
  K=A
else
  if type(A)==10 then
    A=mstr2sci(A)
  end
  K=A<>0
end
endfunction
