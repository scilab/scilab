// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function val=mtlb_realmin(prec)
    // Emulation function for Matlab realmin()

    rhs=argn(2);

    if rhs<=0 then
        val=number_properties("tiny");
    elseif rhs==1 then
        if prec=="double" then
            val=number_properties("tiny");
        else
            error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_realmin"));
        end
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"),"mtlb_realmin",0,1));
    end
endfunction
