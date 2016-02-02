// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


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
