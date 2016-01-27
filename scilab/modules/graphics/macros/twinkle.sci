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
// === LICENSE_END ===

function twinkle(h,n)

    [lhs,rhs]=argn(0)
    if rhs==1 then
        if (type(h)<>9 | size(h,"*")<>1) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Graphic handle expected.\n"), "twinkle", 1));
        end
        n=5;
    elseif rhs==2 then
        if (type(h)<>9 | size(h,"*")<>1) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Graphic handle expected.\n"), "twinkle", 1));
        end
        if (type(n)<>1 | size(n,"*")<>1) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"), "twinkle", 2));
        end
    else
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "twinkle", 1, 2));
    end

    f=h;
    while f.type<>"Figure"
        f=f.parent;
    end
    realtimeinit(0.2);
    realtime(0);
    k=0;

    v=h.visible;
    for i=1:n
        k=k+2;
        h.visible="off";
        realtime(k);
        h.visible="on";
        realtime(k+1);
    end
    h.visible=v;

endfunction
