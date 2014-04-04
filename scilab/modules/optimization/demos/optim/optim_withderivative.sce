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
