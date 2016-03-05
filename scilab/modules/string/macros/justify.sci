// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) DIGITEO - 2010 - Pierre Marechal
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function t = justify(t, job)

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"justify",2))
    end

    if t == [] then
        t = [];
        return;
    end
    if type(t) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "justify", 1));
    end

    if type(job) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "justify", 2));
    end

    if size(job,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"), "justify", 2));
    end

    if and(job <> ["l" "c" "r" "left" "center" "right"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ""%s"", ""%s"" or ""%s"" expected.\n"), "justify", 2,"r","l","c"));
        //  "left,center & right" are just here for backward compatibility
    end

    // Redefine the wanted justification
    // =========================================================================
    job = part(job, 1);

    // Remove leading and trailing whitespaces (See bug #7751)
    // =========================================================================
    t = stripblanks(t);

    //Justify character array.
    [m, n] = size(t);

    for k = 1:n
        L  = length(t(:,k));
        mx = max(L);
        if job == "l" then           //right
            t(:,k) = part(t(:,k),1:mx)
        elseif job == "r" then       //left
            for l = 1:m
                t(l,k) = part(" ", 1:(mx-length(t(l,k)))) + t(l, k);
            end
        elseif job == "c" then       //center
            for l = 1:m
                nl = int((mx - length(t(l, k)))/2)
                nr = (mx - length(t(l, k))) - nl
                t(l, k) = part(" ", 1:nl) + t(l, k) + part(" ", 1:nr);
            end
        end
    end

endfunction
