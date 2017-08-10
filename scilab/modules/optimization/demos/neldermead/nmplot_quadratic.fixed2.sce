// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Michael Baudin
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

function demo_nmplot_qfixed2()

    filename = "nmplot_quadratic.fixed2.sce";
    dname = get_absolute_file_path(filename);

    mprintf(_("Illustrates that the fixed-shape Spendley et al. algorithm performs badly on a badly quadratic test case.\n"));
    mprintf(_("Defining quadratic function...\n"));
    a = 100;

    function [ y , index ] = quadratic ( x , index )
        y = a * x(1)^2 + x(2)^2;
    endfunction
    function y = quadraticC ( x1 , x2 )
        y = quadratic ( [x1 , x2] , 2 )
    endfunction

    mprintf(_("Creating nmplot object ...\n"));
    nm = nmplot_new ();
    nm = nmplot_configure(nm, "-numberofvariables",2);
    nm = nmplot_configure(nm, "-function",quadratic);
    nm = nmplot_configure(nm, "-x0",[10.0 10.0]');
    nm = nmplot_configure(nm, "-maxiter",400);
    nm = nmplot_configure(nm, "-maxfunevals",400);
    nm = nmplot_configure(nm, "-tolxmethod",%f);
    nm = nmplot_configure(nm, "-tolsimplexizerelative",1.e-8);
    nm = nmplot_configure(nm, "-simplex0method","spendley");
    nm = nmplot_configure(nm, "-method","fixed");

    //
    // Setup output files
    //
    simplexfn = TMPDIR + filesep() + "history.simplex.txt";
    nm = nmplot_configure(nm, "-simplexfn",simplexfn);

    //
    // Perform optimization
    //
    mprintf(_("Searching (please wait) ...\n"));
    nm = nmplot_search(nm);
    //
    // Print a summary
    //
    exec(fullfile(dname,"nmplot_summary.sci"),-1);
    nmplot_summary(nm)
    //
    // Plot the contours of the cost function and the simplex history
    mprintf(_("Plotting contour (please wait) ...\n"));
    nm = nmplot_configure(nm, "-verbose", 0);
    xmin = -5.0 ;
    xmax = 12.0 ;
    ymin = -2.0 ;
    ymax = 12.0 ;
    nx = 50 ;
    ny = 50;
    scf()
    drawlater();
    xdata=linspace(xmin,xmax,nx);
    ydata=linspace(ymin,ymax,ny);
    contour ( xdata , ydata , quadraticC , [10.0 50 100 1000 2000 5000 10000 20000] )
    nmplot_simplexhistory ( nm );
    drawnow();
    demo_viewCode(filename);

    // Clean-up
    deletefile(simplexfn);
    nm = nmplot_destroy(nm);
    mprintf("End of demo.\n");
endfunction

demo_nmplot_qfixed2();
clear demo_nmplot_qfixed2;




