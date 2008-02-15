function [j]=jmat(n,m)
//j=jmat(n,m)
//This macro permutes block rows or block columns of a matrix
//
//   n   : number of block rows or block columns of the matrix
//   m   : size of the (square) blocks
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
j=[];
for k=1:n,j(k,n-k+1)=1;end;
j=j.*.eye(m,m);
endfunction
