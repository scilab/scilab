// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = deff(varargin)
    [lhs, rhs] = argn();
    if rhs < 1 | rhs > 2  then
        msg = _("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, "deff", 1, 2))
    end
    if rhs == 2 then    // Usual deff syntax
        prototype = varargin(1)
        text = matrix(varargin(2), -1, 1)
    else
        text = matrix(varargin(1), -1, 1)
        if size(text,"*")==1
            // We must split the input into the prototype and the definition
            // The prototype is ended with the first ")":
            [b, e, prototype] = regexp(text,"/.*?\)/", "o")
            text = part(text,e+1:$)
            // Case r = fun(x,y) x^2+y (single output, no r= in the definition:
            if grep(prototype, "=") <> [] & grep(text,"=")==[]
                outvar = strtok(prototype, "=")
                if grep(outvar,",")==[]
                    text = outvar + " = " + text
                end
            end
        else
            prototype = varargin(1)(1)
            text = varargin(1)(2:$)
        end
    end
    // Detect @ in the prototype and replace it, if any (can't be a function's name):
    if grep(prototype, "@") <> [] // "/(?:[^(]*?)=\s*(.+?)(?:\s*\()/")
        prototype = strsubst(prototype, "@", "%funcname");
    end

    // try to build execstr string
    str = ["function " + prototype ; text ; "endfunction"];
    execstr(str);

    %deff_func = funclist();
    if size(%deff_func, "*") <= 0 | size(%deff_func, "*") <> 1 then
        return
    else
        if argn(1) == 0
            execstr(%deff_func + "= return(" + %deff_func + ")");
        else
            execstr("varargout = list(" + %deff_func + ");")
        end
    end
endfunction
