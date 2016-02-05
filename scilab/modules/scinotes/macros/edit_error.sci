//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
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

function answ = edit_error(clearerror)
    if (argn(2) == 0) then
        clearerror = %f;
    end
    [str, n, line, func] = lasterror(clearerror);
    if (n > 0) then
        if (func == "") then
            answ = gettext("The last error did not occur inside a Scilab macro.");
        else
            p = get_function_path(func);
            if (p <> []) then
                editor(p, line, func);
                answ = gettext("Opening file ") + p + gettext(" on line ") + ..
                msprintf("%d",line) + gettext(" of the source file of the function ") + func;
            else
                answ = gettext("The source file of the function ") + func + ..
                gettext(" was not found in any library path");
            end
        end
    else
        answ = gettext("There is no recorded error");
    end
endfunction

