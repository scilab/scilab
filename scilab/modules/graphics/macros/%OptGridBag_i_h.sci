// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2104 - Scilab Enterprises - Antoine ELIAS
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function h = %OptGridBag_i_h(i,v,h)
    if type(i)==10 then
        set(h,i,v)
    else
        error(msprintf(_("%s: Wrong type for input argument #%d.\n"),"%OptGridBag_i_h",1));
    end
endfunction
