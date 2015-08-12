// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function head_comments(name, %paths)
    // Displays the first comments of a function

    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "head_comments", 1, 2));
    end

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A character string expected.\n"),"head_comments",1))
    end
    name = stripblanks(name)
    if execstr("var="+name,"errcatch")<>0 then
        error(msprintf(gettext("%s: Undefined variable %s.\n"),"head_comments",name )),
    end

    if type(var) == 11 then comp(var); end
    if type(var) <> 13 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Name of a Scilab function expected.\n"),"head_comments",1))
    end
    ops = macr2lst(var);
    syntax = "function ["+strcat(ops(2),",")+"] = "+name+"("+strcat(ops(3),",")+")";

    comm = [];
    k = 4; if ops(4) == "15" then k = k+1; end
    while ops(k)(1)=="31" then
        comm = [comm; ops(k)(2)];
        k = k+2;
    end
    if comm == [] then
        mprintf("%s\n",[syntax;gettext("No comment available.")])
    else
        mprintf("%s\n",[syntax;comm])
    end

endfunction
