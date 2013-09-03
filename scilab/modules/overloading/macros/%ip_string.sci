// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2012 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
