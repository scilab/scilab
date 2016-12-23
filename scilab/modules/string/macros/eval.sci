// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function _h = eval(z)
    // Syntax :  H = eval(Z)
    // returns the evaluation of the matrix of character strings Z.
    // Example: a=1; b=2; Z=['a','b'] ; eval(Z) returns the matrix [1,2];

    [nL,nC] = size(z);
    if nL * nC > 1 then
        L = (1:nL)'*ones(1,nC)
        C = ones(nL,1)*(1:nC)
        //str = "_h("+string(L)+","+string(C)+")="+z    // format()-dependent
        str = msprintf("_h(%d,%d)=%s\n",L(:),C(:),z(:)) // format()-independent
    elseif z~=[]
        str = "_h = " + z;
    else
        _h = []
        return
    end
    deff("_h = %eval();", str)
    _h = %eval()

endfunction
