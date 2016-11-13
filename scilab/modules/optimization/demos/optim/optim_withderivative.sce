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

function demo_derivative()

    mprintf(_("Running optimization ...\n"));

    function f = rosenbrock ( x )
        f = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
    endfunction

    function [ f , g , ind ] = rosenbrockCost2 ( x , ind )
        if ind == 1 | ind == 4 then
            f = rosenbrock ( x );
            g = numderivative ( rosenbrock , x.' , [], order = 4 );
        end
    endfunction

    x0 = [-1.2 1];
    [ f , x ] = optim ( rosenbrockCost2 , x0 );

    //
    // Display results
    //
    mprintf("x = %s\n",strcat(string(x)," "));
    mprintf("f = %e\n",f);

    //
    // Load this script into the editor
    //
    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if m == 1
        filename = "optim_withderivative.sce";
        dname = get_absolute_file_path(filename);
        editor ( dname + filename );
    end
endfunction


demo_derivative();
clear demo_derivative;
