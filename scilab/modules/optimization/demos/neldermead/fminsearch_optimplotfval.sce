// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Michael Baudin
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function demo_plotfval()

    mprintf(_("Running optimization ...\n"));

    function y = banana (x)
        y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
    endfunction

    opt = optimset ( "PlotFcns" , optimplotfval );
    [x fval] = fminsearch ( banana , [-1.2 1] , opt );
    demo_viewCode("fminsearch_optimplotfval.sce");
    //
    // Display results
    //
    mprintf("x = %s\n",strcat(string(x)," "));
    mprintf("fval = %e\n",fval);
endfunction

demo_plotfval();
clear demo_plotfval;






