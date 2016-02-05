//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Bernard DUJARDIN <develop.dujardin@numericable.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

function block_parameter_error(inform, expected)
    // Displays a parameter input error box for an Xcos block
    // Parameters
    // inform: a string, type of the error
    // expected: a string, expected value

    rhs=argn(2)
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "block_parameter_error", 2))
    end
    if typeof(inform) <> "string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "block_parameter_error", 1 ))
    end
    if typeof(expected) <> "string"
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"block_parameter_error", 2));
    end

    boxmsg = [" "; inform; " "; expected];
    messagebox(boxmsg, gettext("Xcos block parameter error"), "error","modal");
endfunction
