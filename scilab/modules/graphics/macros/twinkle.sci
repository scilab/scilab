// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
