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

function val=mtlb_realmax(prec)
    // Emulation function for Matlab realmax()

    rhs=argn(2);

    if rhs<=0 then
        val=number_properties("huge");
    elseif rhs==1 then
        if prec=="double" then
            val=number_properties("huge");
        else
            error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_realmax"));
        end
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"),"mtlb_realmax",0,1));
    end
endfunction
