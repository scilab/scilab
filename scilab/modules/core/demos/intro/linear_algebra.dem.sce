//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
mode(7)
//To quit this demo, enter CTRL+C, then type "abort"

//               LINEAR ALGEBRA
//                 PRIMITIVES
H=[1.    1.    1.    0.;
2.   -1.    0.    1;
1.    0.    1.    1.;
0.    1.    2.   -1];

ww=spec(H)

//             STABLE SUBSPACES
[X,d]=schur(H,"cont");
X'*H*X

[X,d]=schur(H,"disc");
X'*H*X

//Selection of user-defined eigenvalues (# 3 and 4 here);
function [flg]=sel(x)
    flg=%f
    if abs(x-ww(3))<0.0001|abs(x-ww(4))<0.00001 then flg=%t,end
endfunction

[X,d]=schur(H,sel)

X'*H*X
clear sel

//               With matrix pencil
function [flg]=sel(x,y)
    flg=%f
    if abs(x/y-ww(3))<0.0001|abs(x/y-ww(4))<0.00001 then flg=%t,end
endfunction

[X,d]=schur(H,eye(H),sel)
X'*H*X

//            block diagonalization
[ab,x,bs]=bdiag(H);

inv(x)*H*x

//                     Matrix pencils
E=rand(3,2)*rand(2,3);
A=rand(3,2)*rand(2,3);
s=poly(0,"s");

w=det(s*E-A)   //determinant
[al,be]=spec(A,E);
al./(be+%eps*ones(be))
roots(w)
[Ns,d]=coffg(s*E-A);    //inverse of polynomial matrix;
clean(Ns/d*(s*E-A))
[Q,M,i1]=pencan(E,A);   // Canonical form;
clean(M*E*Q)
clean(M*A*Q)
