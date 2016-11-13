// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

    if and(type(var) <> [11 13]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Name of a Scilab function expected.\n"),"head_comments",1))
    end
    t = macr2tree(var);
    outputs = [];
    for k=1:size(t.outputs)
        outputs=[outputs,expression2code(t.outputs(k))]
    end
    inputs = [];
    for k=1:size(t.inputs)
        inputs=[inputs,expression2code(t.inputs(k))]
    end
    if isempty(outputs) then
        outputs = '';
    end
    if isempty(inputs) then
        inputs = '';
    end
    syntax = "function ["+strcat(outputs,",")+"] = "+name+"("+strcat(inputs,",")+")";

    comm = [];
    statements = t.statements
    k = 1;
    if statements(1) == list("EOL") then
        k = k+1;
    end
    while typeof(statements(k))=="comment" then
        comm = [comm; statements(k).text];
        k = k+2;
    end
    if comm == [] then
        mprintf("%s\n",[syntax;gettext("No comment available.")])
    else
        mprintf("%s\n",[syntax;comm])
    end

endfunction
