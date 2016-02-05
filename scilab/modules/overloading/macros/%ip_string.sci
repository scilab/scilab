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

function r = %ip_string(index)
    r = []; v = [];
    if execstr("v = [index(1) index(2) index(3)];", "errcatch") == 0 then
        r = sci2exp(v(1)) + ":" + sci2exp(v(2)) + ":" + sci2exp(v(3));
    else
        if execstr("v = [index(1) index(2)];", "errcatch") == 0 then
            r = sci2exp(v(1)) + ":" + sci2exp(v(2));
        else
            error(999, msprintf(gettext("%s: Wrong size for input argument #%d.\n"), "string", 1));
        end
    end
endfunction
