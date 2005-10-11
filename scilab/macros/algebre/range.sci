function [X,dim]=range(A,k)
// Computation of Range A^k ; the first dim columns of X span the
// range of A^k.
//F.D.
//!
// Copyright INRIA
  if k==0
    [n,n]=size(A);  
    dim=n;X=eye(A);return;
  end
  [U,dim]=rowcomp(A);X=U;
  if k==1
    return;
  end
  for l=2:k
    A=A*U';
    [U,dim]=rowcomp(A(:,1:dim));
    X=U*X;
  end;
endfunction
