// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [pop_out] = coding_ga_identity(pop_in,direction,param)
    // pop_in is a list().
    // pop_out must be a list().
    if (direction=="code") then
        pop_out = pop_in;
    elseif (direction=="decode") then
        pop_out = pop_in;
    else
        error(sprintf(gettext("%s: wrong direction"),"coding_ga_identity"));
    end
endfunction
