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

function y=mtlb_l(a,b)
    // Emulation function for Matlab left division

    if type(a)==10 then
        a=asciimat(a)
    end
    if type(b)==10 then
        b=asciimat(b)
    end

    if or(type(a)==[4,6]) then
        a=bool2s(a)
    end
    if or(type(b)==[4,6]) then
        b=bool2s(b)
    end

    y=a\b

endfunction
