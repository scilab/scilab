function [X,dim]=range(A,k)
// Computation of Range A^k ; the first dim rows of X span the
// range of A^k.
//F.D.
//!
// Copyright INRIA
  if argn(2)==1 then k=1,end
  k=double(k)
  if int(k)<>k|k<0 then 
    error('range: second argument should be a non negative  integer')
  end
  if size(A,1)<>size(A,2)|~isreal(A) then
     error('range: first argument should be a real square matrix')
  end
  
  if k==0 then
    dim=size(A,1);X=eye(A);
  else
    [U,dim]=rowcomp(A);X=U;
    for l=2:k
      A=A*U';
      [U,dim]=rowcomp(A(:,1:dim));
      X=U*X;
    end;
  end
endfunction
