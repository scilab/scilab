// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2013 - Samuel GOUGEON : extend to 3D + set of handles + skip with %nan

// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function replot(rect,handl)
    // replots the graphic axes using rect as x and y (and z) bounds
    //

    [lhs,rhs] = argn(0);

    if rhs < 1 then
        msg = _("%s: Wrong number of input argument(s): %d or %d expected.\n")
        error(msprintf(msg, "replot", 1, 2));
    end

    if rhs == 1
        a = gca();
    else
        if (type(handl) == 9)
            if and(handl.type == "Axes")
                a = handl;
            else
                msg = _("%s: Input argument #%d must be a handle on an axis.\n")
                error(msprintf(msg, "replot", 2));
            end
        else
            msg = _("%s: Wrong type for input argument #%d: Graphic handle expected.\n")
            error(msprintf(msg, "replot", 2));
        end
    end
    if length(rect)==4 then
        rect = [rect(1) rect(2) ; rect(3) rect(4)]
    elseif length(rect)==6
        rect = [rect(1) rect(2) rect(3); rect(4) rect(5) rect(6)]
    end
    k = ~isnan(rect)

    for i = 1:length(a)
        if length(rect)==4 then
            a(i).data_bounds(k) = rect(k)
        elseif length(rect)==6 & a(i).view=="3d"
            a(i).data_bounds(k) = rect(k)
        end
    end
endfunction
