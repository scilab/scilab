// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
