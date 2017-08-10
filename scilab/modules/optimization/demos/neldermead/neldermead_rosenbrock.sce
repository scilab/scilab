// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
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

function demo_nelder_rosen()

    mprintf(_("Defining Rosenbrock function ...\n"));

    function [ y , index ] = rosenbrock ( x , index )
        y = 100*(x(2)-x(1)^2)^2+(1-x(1))^2;
    endfunction

    x0 = [-1.2 1.0]';

    nm = neldermead_new();
    nm = neldermead_configure(nm, "-numberofvariables", 2);
    nm = neldermead_configure(nm, "-function", rosenbrock);
    nm = neldermead_configure(nm, "-x0", x0);
    nm = neldermead_configure(nm, "-maxiter", 300);
    nm = neldermead_configure(nm, "-maxfunevals", 300);

    mprintf(_("Searching (please wait) ...\n"));
    nm = neldermead_search(nm);

    fx0 = neldermead_get(nm, "-fx0");
    mprintf("f(x0) = %f, x0=[%s]\n" , fx0 , strcat(string(x0), " "));
    xopt = neldermead_get(nm, "-xopt");
    fopt = neldermead_get(nm, "-fopt");
    mprintf("f(xopt) = %f, xopt=[%s]\n" , fopt , strcat(string(xopt), " "));
    nm = neldermead_destroy(nm);
    mprintf(_("End of demo.\n"));

    //
    // Load this script into the editor
    //
    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if(m == 1)
        filename = "neldermead_rosenbrock.sce";
        dname = get_absolute_file_path(filename);
        editor ( dname + filename, "readonly" );
    end
endfunction

demo_nelder_rosen();
clear demo_nelder_rosen;







