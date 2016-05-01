// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [a]=sca(h)
    //set default axes, h must be a handle

    [lhs,rhs] = argn(0);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "sca",1));
        return;
    end

    if (type(h) <> 9) | (h.type <> "Axes") then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An ''%s'' handle expected.\n"), "sca",1, "Axes"));
        return;
    end

    if (size(h) <> [1,1]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: An ''%s'' handle expected.\n"), "sca",1, "Axes"));
        return;
    end

    set("current_axes", h);
    a=get("current_axes");

endfunction
