// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [x] = input(msg, flag)

    [lhs,rhs] = argn(0);

    if rhs <> 1 & rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "input", 1, 2));
    end

    if type(msg) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"input",1));
    end

    if size(msg, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"input",1));
    end

    // a tricky way to get all ascii codes  sequences
    fmt = "%[" + ascii(32) + "-" + ascii(254) + "]";

    currentprompt = prompt();

    if argn(2) == 2 then
        if type(flag) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"input",2));
        end

        if size(flag, "*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"input",2));
        end

        if (flag <> "s" & flag <> "string") then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' value expected.\n"),"input",2,"string"));
        end

        prompt(msg);
        x = mscanf(fmt);

        currentpromptAfter = prompt();
        // bug 5513
        // we had change prompt during exec of input
        // we recall input
        if (currentpromptAfter <> currentprompt) then
            x = input(msg, flag);
        end

    else
        while %t
            prompt(msg);
            x = mscanf(fmt);

            currentpromptAfter = prompt();
            // bug 5513
            // we had change prompt during exec of input
            // we recall input
            if (currentpromptAfter <> currentprompt) then
                x = string(input(msg));
            end

            if (length(x) == 0) | (x == " ") then
                x = "[]";
            end
            ierr = execstr("x=" + x,"errcatch");
            if ierr == 0 then
                break;
            end
            mprintf(strcat(lasterror(),"\n"));
        end

    end
    prompt(currentprompt);
endfunction
