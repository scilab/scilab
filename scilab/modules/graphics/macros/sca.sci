// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [a]=sca(h)
    //set default axes, h must be a handle

    [lhs,rhs] = argn(0);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "sca",1));
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
