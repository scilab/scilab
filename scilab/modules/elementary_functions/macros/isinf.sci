// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
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

function r = isinf(x)

    rhs = argn(2);

    if rhs <> 1 then
        msg = _("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg, "isinf", 1))
    end

    if x==[] then
        r = []
    else
        if isreal(x)
            r = abs(x)==%inf;
        else
            // workaround of http://bugzilla.scilab.org/14062
            r = abs(real(x))==%inf | abs(imag(x))==%inf
        end
    end

endfunction
