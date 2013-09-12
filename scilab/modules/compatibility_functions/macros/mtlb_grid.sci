// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
