function [U,dim]=range(A,k)
// Computation of Range A^k ; the first dim columns of U' span the
// range of A^k.
//F.D.
//!
// Copyright INRIA
  if argn(2)==1 then k=1,end
  if k==0
    [n,n]=size(A);  
    dim=n;U=eye(n,n);return;
  end
  [U,dim]=rowcomp(A);
  if k==1
    return;
  end
  if size(A,1)<>size(A,2) then
     error('range: first argument should be a square matrix')
  end
  for l=2:k
    B=A*U';
    [U,dim]=rowcomp(B(:,1:dim));
  end;
endfunction
