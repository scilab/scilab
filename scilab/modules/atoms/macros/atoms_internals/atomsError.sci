// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Internal function

function atomsError(issue,msg)

    // Check input argments
    // =========================================================================

    rhs = argn(2);

    // Number of input argments

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsError",2))
    end

    // 1st input argument

    if type(issue) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsError",1));
    end

    if size(issue,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsError",1));
    end

    if and( issue<>["error","warning"] ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d:''error'' or ''warning'' expected.\n"),"atomsError",1));
    end

    // 2nd input argument

    if type(msg) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsError",2));
    end

    // Display the error message
    // =========================================================================

    for i=1:size(msg,"*")
        mprintf(msg(i)+"\n");
    end

    // Go to the initial path if defined
    // =========================================================================
    if isdef("ATOMSINITIALPATH") & ~ isempty(ATOMSINITIALPATH) & isdir(ATOMSINITIALPATH) then
        chdir(ATOMSINITIALPATH);
    end

    // Throw error or warning
    // =========================================================================

    if issue == "error" then
        error("");

    elseif issue == "warning" then
        warning("");

    end


endfunction
