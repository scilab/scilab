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


















