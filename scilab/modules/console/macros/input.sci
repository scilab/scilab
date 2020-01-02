// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x] = input(msg, flag)

    [lhs,rhs] = argn(0);

    if rhs <> 1 & rhs <> 2 then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "input", 1, 2));
    end

    if type(msg) <> 10 then
        msg = _("%s: Wrong type for input argument #%d: String expected.\n")
        error(msprintf(msg, "input",1));
    end

    if size(msg, "*") <> 1 then
        msg = _("%s: Wrong size for input argument #%d: string expected.\n")
        error(msprintf(msg, "input",1));
    end

    // a tricky way to get all ascii codes  sequences
    fmt = "%[" + ascii(32) + "-" + ascii(254) + "]";

    if argn(2) == 2 then
        if type(flag) <> 10 then
            msg = _("%s: Wrong type for input argument #%d: String expected.\n")
            error(msprintf(msg, "input",2));
        end

        if size(flag, "*") <> 1 then
            msg = _("%s: Wrong size for input argument #%d: string expected.\n")
            error(msprintf(msg, "input",2));
        end

        if (flag <> "s" & flag <> "string") then
            msg = _("%s: Wrong value for input argument #%d: ''%s'' value expected.\n")
            error(msprintf(msg, "input", 2, "string"));
        end

        prompt(msg);
        x = mscanf(fmt);
    else
        while %t
            prompt(msg);
            __#x#__ = mscanf(fmt);

            if (length(__#x#__) == 0) | (__#x#__ == " ") then
                __#x#__ = "[]";
            end

            ierr = execstr("x=" + __#x#__,"errcatch");

            if ierr == 0 then
                break;
            end
            mprintf(lasterror());
            mprintf("\n");
        end
    end

    mprintf("\n");
endfunction
