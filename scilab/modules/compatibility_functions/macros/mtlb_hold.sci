// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
