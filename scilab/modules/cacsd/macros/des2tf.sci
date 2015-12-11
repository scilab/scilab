// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [Bfs,Bis,tf]=des2tf(des)
    //des admits a D matrix.

    [LHS,RHS]=argn(0);
    if LHS<>1 & LHS<> 3 then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d or %d expected.\n"), "des2tf",1,3));
    end
    [A,B,C,D,E]=des(2:6);
    [Bfs,Bis,chis]=glever(E,A);
    if LHS==3 then Bfs=C*Bfs*B; Bis=C*Bis*B+D;tf=chis;return;end
    if LHS==1 then ww=C*Bfs*B;Bfs=ww/chis-C*Bis*B+D;return;end
endfunction
