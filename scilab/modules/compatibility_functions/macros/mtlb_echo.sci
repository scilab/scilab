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

function mtlb_echo(arg1,arg2)
    // Emulation function for Matlab echo()

    rhs=argn(2);

    if rhs<=0 then
        mode(abs(mode()-1))
    elseif rhs==1 then
        if arg1=="on" then
            mode(1);
        elseif arg1=="off" then
            mode(0);
        else
            error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_echo"));
        end
    else
        error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_echo"));
    end
endfunction
