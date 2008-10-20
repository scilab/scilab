function [Q,R,E]=mtlb_qr(A,B)
// Copyright INRIA
// Emulation function for qr() Matlab function
// V.C.

[lhs,rhs]=argn()

if lhs<>3 then
  error("Not yet implemented for "+string(lhs)+" lhs argument(s) !")
end
if rhs<>2 then
  error("Not yet implemented "+string(rhs)+" rhs argument(s) !")
end  

[Q,R,E] = qr(A)
Q = Q(:,1:min(size(A,2),size(Q,2)))
if find(size(A)==1)<>[] then
  R = R(1,1:size(A,2))
end
E = (1:size(R,2))*E

endfunction
