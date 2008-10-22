// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [Y,rk,Ac]=colcompr(A);
//[Y,rk,Ac]=colcompr(A);
//column compression of polynomial matrix A
//(left compression)
//Y = right unimodular base
//rk = normal rank of A
//Ac = A*Y
//see rowcompr
//!
[m,n]=size(A);
[Ac,Y,rk]=htrianr(A);
endfunction
