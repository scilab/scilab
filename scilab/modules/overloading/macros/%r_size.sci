// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2012 - Allan CORNET
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = %r_size(x, flag)
    // only to be called by size()

    [lhs,rhs] = argn(0)
    x = x.num;
    if lhs == 1 then
        if rhs == 1 then
            m = size(x)
        else
            m = size(x, flag)
        end
        varargout = list(m)
    else
        if rhs > 1 then
            msg = _("%s: Wrong number of output arguments: %d expected.\n");
            error(msprintf(msg, "%r_size", 1));
        end
        s = size(x);
        execstr("varargout = list("+strcat(msprintf("%d\n",s'), ",")+")")
    end
endfunction
