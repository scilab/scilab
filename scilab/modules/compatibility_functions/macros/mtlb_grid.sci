// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function mtlb_grid(arg1,arg2)
    // Emulation function for Matlab grid

    rhs=argn(2)
    if rhs<=0 then // toggle
        a=gca();
        if and(a.grid==[-1 -1]) then
            a.grid=[1 1]
        else
            a.grid=[-1 -1]
        end
    elseif rhs==1 then
        if type(arg1)==9 then // toggle
            if and(arg1.grid==[-1 -1]) then
                arg1.grid=[1 1]
            else
                arg1.grid=[-1 -1]
            end
        else
            a=gca();
            if arg1=="on" then
                a.grid=[1 1]
            elseif arg1=="off" then
                a.grid=[-1 -1]
            elseif arg1=="minor" then
                warning(msprintf(gettext("%s: No minor grid in Scilab.\n"), "mtlb_grid"));
                if and(a.grid==[-1 -1]) then
                    a.grid=[1 1]
                else
                    a.grid=[-1 -1]
                end
            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'' or ''%s'' expected.\n"), "mtlb_grid", 1, "on", "off", "minor"));
            end
        end
    elseif rhs==2 then
        if arg2=="on" then
            arg1.grid=[1 1]
        elseif arg2=="off" then
            arg1.grid=[-1 -1]
        elseif arg2=="minor" then
            warning(msprintf(gettext("%s: No minor grid in Scilab.\n"), "mtlb_grid"));
            if and(arg1.grid==[-1 -1]) then
                arg1.grid=[1 1]
            else
                arg1.grid=[-1 -1]
            end
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'' or ''%s'' expected.\n"), "mtlb_grid", 2, "on", "off", "minor"));
        end
    end
endfunction
