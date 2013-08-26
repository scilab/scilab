// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [K,Y,err]=leqe(P21,Qx)
    if typeof(P21)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"leqe",1))
    end

    [A,B1,C2,D21,xo,dom]=P21(2:7)
    [KT,Y,err]=leqr(syslin(dom,A',C2',B1',D21'),Qx);
    K=KT';
endfunction
