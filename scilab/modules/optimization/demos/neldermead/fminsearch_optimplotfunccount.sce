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

function demo_funccount()

    mprintf(_("Running optimization ...\n"));

    function y = banana (x)
        y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
    endfunction

    opt = optimset ( "PlotFcns" , optimplotfunccount );
    [x fval] = fminsearch ( banana , [-1.2 1] , opt );
    demo_viewCode("fminsearch_optimplotfunccount.sce");

    //
    // Display results
    //
    mprintf("x = %s\n", strcat(string(x)," "));
    mprintf("fval = %e\n", fval);

endfunction

demo_funccount();
clear demo_funccount;






