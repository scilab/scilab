// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Internal function

function atomsDisp(str)

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsDisp",1,2))
    end

    if type(str) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsDisp",1));
    end

    // Verbose Mode ?
    // =========================================================================
    if size(str,"*")>1 | length(str($))>0 & part(str($),$)==ascii(10)
        form = "%s\n"
        str = str(:)
    else
        form = "%s"
    end
    if isdef("ATOMSVERBOSE") then
        if ATOMSVERBOSE then
            mprintf(form,str);
        end
    elseif strcmp(atomsGetConfig("Verbose"),"True", "i") == 0
        mprintf(form, str);
    end

endfunction
