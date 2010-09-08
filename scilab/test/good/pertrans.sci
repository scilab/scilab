// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function W=pertrans(A)
// W=pertranspose(A)

[n,m]=size(A);
A=A';
W=A(m:-1:1,n:-1:1);
endfunction
