//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

