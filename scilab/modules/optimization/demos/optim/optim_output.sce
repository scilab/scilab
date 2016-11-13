// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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








