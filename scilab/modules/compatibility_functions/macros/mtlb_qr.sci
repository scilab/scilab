function [Q,R,E]=mtlb_qr(A,B)
// Copyright INRIA
// Emulation function for qr() Matlab function
// V.C.

[lhs,rhs]=argn()

if lhs<>3 then
  error(msprintf(gettext("errors","Not yet implemented for %d lhs argument(s)."),lhs));
end
if rhs<>2 then
  error(msprintf(gettext("errors","Not yet implemented for %d rhs argument(s)."),rhs));
end  

[Q,R,E] = qr(A)
Q = Q(:,1:min(size(A,2),size(Q,2)))
if find(size(A)==1)<>[] then
  R = R(1,1:size(A,2))
end
E = (1:size(R,2))*E

endfunction
