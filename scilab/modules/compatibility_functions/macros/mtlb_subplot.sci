// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function mtlb_subplot(m,n,p)
    // Copyright INRIA
    [lhs,rhs]=argn(0)
    if rhs==1 then
        p=modulo(m,10)
        n=modulo((m-p)/10,10)
        m=(m-p-10*n)/100
    end
    j=int((p-1)/n)
    i=p-1-n*j
    xsetech([i/n,j/m,1/n,1/m])
endfunction
