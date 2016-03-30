// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// This function is to warn user that he's using a deprecated function

function warnobsolete(newFunctionName, scilabRemovedVersion)
    // Modal warning is to set if this function
    // should block execution or only print
    // on the standard outstream
    global %modalWarning

    //Retrieve Calling function
    [lineCall, stackCall]=where()
    if size(stackCall,"*") < 2 then
        error(msprintf(gettext("%s: Private function: cannot access to this function directly.\n"),"warnobsolete"));
    end
    // Now build the correct warning message
    warnMessage = msprintf(_("Feature %s is obsolete."),stackCall(2))
    if exists("newFunctionName", "l") then
        warnMessage = [warnMessage, msprintf(_("Please use %s instead."),newFunctionName)]
    end
    if exists("scilabRemovedVersion", "l") then
        warnMessage = [warnMessage, msprintf(_("This feature will be permanently removed in Scilab %s"), scilabRemovedVersion)]
    end

    // Now show the warning

    if %modalWarning then
        messagebox(warnMessage,"modal");
    else
        warning(warnMessage);
    end

    clear %modalWarning

endfunction
