function [X,rk,Ac]=rowcompr(A)
//[X,rk,Ac]=rowcompr(A)
//row compression of polynomial matrix A (toward the bottom)
//X left polynomial unimodular base
//rk=normal rank of A
//Ac = X*A
//Warning: elimination of neglected terms
//!
// Copyright INRIA
[n,m]=size(A);
[Ac,U,rk]=htrianr(A');
Ac=Ac';
X=U';
X=X(n:-1:1,:)
Ac=Ac(n:-1:1,:)
endfunction
