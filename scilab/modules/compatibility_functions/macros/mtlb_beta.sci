function [Y]=mtlb_beta(A,B)
// Copyright INRIA
// Emulation function for Matlab beta()
// V.C.

if (size(A,"*")==0 & size(B,"*")==0) | (size(A,"*")==0 & type(B)==1) | (size(B,"*")==0 & type(A)==1) then
Y=[]
elseif size(A,"*")==1 & size(B,"*")<>1 then
  tmp=A
  A=B
  A(:)=tmp
  Y=beta(A,B)
elseif size(A,"*")<>1 & size(B,"*")==1 then
  tmp=B
  B=A
  B(:)=tmp
  Y=beta(A,B)
  else
   Y=beta(A,B)
end

endfunction


