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

function X = sylv(A,B,C,flag)
    //  solve  A*X+X*B=C if flag=='c' or  A*X*B+X=C if flag=='d'
    if argn(2)<>4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"sylv",4))
    end
    if size(A,1)<> size(A,2) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "sylv", 1));
    end
    if size(B,1)<> size(B,2) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "sylv", 2));
    end
    if size(C,1)<> size(A,1) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same number of rows expected.\n"),"sylv",1,3))
    end
    if size(C,2)<> size(B,2) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same number of columns expected.\n"),"sylv",2,3))
    end

    if flag=="c" then
        flag=[0 0 0],
    elseif flag=="d" then
        flag=[1 0 0],
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ""c"" or ""d"" expected.\n"), "sylv", 4));
    end
    X=linmeq(1,A,B,C,flag)
endfunction
