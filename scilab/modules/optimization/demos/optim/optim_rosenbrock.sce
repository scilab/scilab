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

function demo_rosenbrock()

    mprintf(_("Running optimization ...\n"));

    function [ f , g , ind ] = rosenbrock ( x , ind )
        f = 100*(x(2)-x(1)^2)^2+(1-x(1))^2;
        g(1) = - 400. * ( x(2) - x(1)**2 ) * x(1) -2. * ( 1. - x(1) )
        g(2) = 200. * ( x(2) - x(1)**2 )
    endfunction

    x0 = [-1.2 1];
    [f, x] = optim(rosenbrock, x0);

    //
    // Display results
    //
    mprintf("x = %s\n", strcat(string(x)," "));
    mprintf("f = %e\n", f);

    //
    // Load this script into the editor
    //
    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if(m == 1)
        filename = "optim_rosenbrock.sce";
        dname = get_absolute_file_path(filename);
        editor ( dname + filename );
    end
endfunction

demo_rosenbrock();
clear demo_rosenbrock;


















