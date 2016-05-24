// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2015 - 2016 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function twinkle(h,n)

    // CHECKING INPUT ARGUMENTS
    [lhs,rhs]=argn(0)
    if rhs==0 then
        if winsid()~=[] then
            h = gce()
            n = 5
        else
            return
        end
    end
    if rhs==1 then
        if type(h)<>9
            n = h
            if winsid()~=[]
                h = gce()
            else
                return
            end
        else
            h = h(1)
            n = 5
        end
    end
    if type(h)<>9 then
        msg = _("%s: Wrong type for input argument #%d: Graphic handle expected.\n")
        error(msprintf(msg, "twinkle", 1));
    end
    if type(n)<>1 | n<0 then
        msg = _("%s: Wrong type for input argument #%d: Positive integer expected.\n")
        error(msprintf(msg, "twinkle", min(rhs,2)));
    end

    // BLINKING THE GRAPHICAL COMPONENT
    f = h;
    while f.type<>"Figure"
        f = f.parent;
    end
    realtimeinit(0.2);
    realtime(0);
    k = 0;

    v = h.visible;
    for i = 1:n
        k = k+2;
        h.visible = "off";
        realtime(k);
        h.visible = "on";
        realtime(k+1);
    end
    h.visible = v;
endfunction
