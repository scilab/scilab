function [Y]=mtlb_beta(A,B)
// Copyright INRIA
// Emulation function for Matlab beta()
// V.C.

if size(A,"*")==1 & size(B,"*")<>1 then
  tmp=A
  A=B
  A(:)=tmp
elseif size(A,"*")<>1 & size(B,"*")==1 then
  tmp=B
  B=A
  B(:)=tmp
end

Y=beta(A,B)

endfunction


