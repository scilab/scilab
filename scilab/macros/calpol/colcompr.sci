function [Y,rk,Ac]=colcompr(A);
//[Y,rk,Ac]=colcompr(A);
//column compression of polynomial matrix A
//(left compression)
//Y = right unimodular base
//rk = normal rank of A
//Ac = A*Y
//see rowcompr
//!
// Copyright INRIA
[m,n]=size(A);
[Ac,Y,rk]=htrianr(A);
endfunction
