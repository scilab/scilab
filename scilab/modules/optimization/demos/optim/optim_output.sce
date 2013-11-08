// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function demo_optim_output()

    mprintf(_("Running optimization ...\n"));

    xref = [1;2;3];
    x0 = [1;-1;1];

    function [f,g,ind] = cost(x,ind)
        if ( ind == 1 | ind == 4 ) then
            f = 0.5*norm(x-xref)^2;
        end

        if ( ind == 1 | ind == 4 ) then
            g = x - xref;
        end

        if ( ind == 1 ) then
            mprintf("===========\n")
            mprintf("x = %s\n", strcat(string(x)," "))
            mprintf("f = %e\n", f)
            g=x-xref;
            mprintf("g = %s\n", strcat(string(g)," "))
        end

    endfunction

    [f, xopt] = optim(cost, x0, imp=-1)
    //
    // Load this script into the editor
    //
    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if(m == 1)
        filename = "optim_output.sce";
        dname = get_absolute_file_path(filename);
        editor ( dname + filename );
    end
endfunction

demo_optim_output()
clear demo_optim_output;








