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

function mtlb_hold(flag)
    // Emulation function for Matlab hold()

    rhs=argn(2)
    if rhs<=0 then
        a=gca();
        if a.auto_clear=="off" then
            a.auto_clear="on"
        else
            a.auto_clear="off"
        end
    else
        if flag=="on" then
            a=gca();a.auto_clear="off"
        elseif flag=="off" then
            a=gca();a.auto_clear="on"
        else
            error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_hold"))
        end
    end
endfunction
