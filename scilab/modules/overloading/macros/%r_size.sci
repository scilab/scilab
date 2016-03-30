// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2012 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [m, n, nx] = %r_size(x, flag)
    // only to be called by size function for dynamical systems
    //!
    m = 0;
    n = 0;
    [lhs,rhs] = argn(0)
    x1 = x(1);
    if lhs == 1 then
        if rhs == 1 then
            execstr("m = size(x(''num''));","errcatch");
        else
            execstr("m = size(x(''num''), flag);","errcatch");
        end
    elseif lhs == 2 then
        if rhs <> 1 then
            error(41)
        end
        execstr("[m, n] = size(x(''num''));","errcatch");
    end
endfunction
