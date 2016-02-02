// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// blanks macro
// blanks(n)
// Create string of blank characters

function str = blanks(n)

    if ~or(type(n)==[1,8]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An integer expected.\n"),"blanks",1));
    end

    if and(size(n)<>[1 1]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"blanks",1));
    end

    if n>0 then
        str = part(" ",ones(1,n));
    elseif n==0
        str = "";
    else
        error(msprintf(gettext("%s: Wrong values for input argument #%d: An non-negative integer expected.\n"),"blanks",1));
    end

endfunction
