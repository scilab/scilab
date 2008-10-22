function [dim,x]=unobs(A,C,tol)
// n first columns of x span the unobservable
// subspace of (A,C):
//          dim  
//          [*,*]
// X'*A*X = [0,*]
// 
//    C*X = [0,*]
// Copyright INRIA
[p,p]=size(A);tol=1.d-10*norm([A;C],1);
[n,w]=contr(A',C',tol);
x=[w(:,n+1:p),w(:,1:n)];
dim=p-n;
endfunction
