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

function X = lyap(A,C,flag)
    //  solve  A'*X+X*A=C if flag=='c' or  A'*X*A-X=C if flag=='d'
    if argn(2)<>3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"lyap",3))
    end
    flag=part(flag,1)
    if flag=="c" then
        flag=[0 0],
    elseif flag=="d" then
        flag=[1 0],
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ""c"" or ""d"" expected.\n"), "lyap", 3));
    end
    X=linmeq(2,A,C,flag)
endfunction
