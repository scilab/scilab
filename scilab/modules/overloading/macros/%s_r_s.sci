// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function R=%s_r_s(B,A)
    // used to give a sense to operation like 1/A where A is a matrix
    n=size(A,2)
    R=(B*eye(n,n))/A
endfunction
