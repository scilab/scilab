// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [X,rk,Ac]=rowcompr(A)
//[X,rk,Ac]=rowcompr(A)
//row compression of polynomial matrix A (toward the bottom)
//X left polynomial unimodular base
//rk=normal rank of A
//Ac = X*A
//Warning: elimination of neglected terms
//!
[n,m]=size(A);
[Ac,U,rk]=htrianr(A');
Ac=Ac';
X=U';
X=X(n:-1:1,:)
Ac=Ac(n:-1:1,:)
endfunction
