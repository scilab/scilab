
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function t=trace(a)
    // trace - computes the trace of a matrix
    select type(a)
    case 1 then
        [m,n]=size(a)
        if m<>n then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"trace",1));
        end
        t=sum(diag(a))
    case 2 then
        [m,n]=size(a)
        if m<>n then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"trace",1));
        end
        t=sum(diag(a))
        //-compat next case retained for list /tlist compatibility
    case 15 then
        if a(1)=="r" then
            [m,n]=size(a)
            if m<>n then
                error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"trace",1));
            end
            t=sum(diag(a))
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"trace",1));
        end
    case 16 then
        if a(1)=="r" then
            [m,n]=size(a)
            if m<>n then
                error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"trace",1));
            end
            t=sum(diag(a))
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"trace",1));
        end
    case 5 then
        [m,n]=size(a)
        if m<>n then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"trace",1));
        end
        t=sum(diag(a))
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"trace",1));
    end
endfunction
