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

function demo_nmplot_rosen()

    filename = "nmplot_rosenbrock.fixed.sce";
    dname = get_absolute_file_path(filename);

    mprintf(_("Illustrates that the fixed-shape Spendley et al. algorithm does NOT perform well on Rosenbrock test case.\n"));
    mprintf(_("Defining Rosenbrock function...\n"));

    function [ y , index ] = rosenbrock ( x , index )
        y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
    endfunction
    function [ y , index ] = rosenbrockC ( x1 , x2 )
        y = rosenbrock ( [x1 , x2] , 2 )
    endfunction

    mprintf(_("Creating nmplot object ...\n"));
    nm = nmplot_new ();
    nm = nmplot_configure(nm, "-numberofvariables",2);
    nm = nmplot_configure(nm, "-function",rosenbrock);
    nm = nmplot_configure(nm, "-x0",[-1.2 1.0]');
    nm = nmplot_configure(nm, "-maxiter",100);
    nm = nmplot_configure(nm, "-maxfunevals",300);
    nm = nmplot_configure(nm, "-tolfunrelative",10*%eps);
    nm = nmplot_configure(nm, "-tolxrelative",10*%eps);
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

    // Plot the contours of the cost function and the simplex history
    mprintf(_("Plotting contour (please wait) ...\n"));
    xmin = -2.0 ;
    xmax = 2.0 ;
    ymin = -2.0 ;
    ymax = 2.0 ;
    nx = 50 ;
    ny = 50;
    xdata=linspace(xmin,xmax,nx);
    ydata=linspace(ymin,ymax,ny);
    f = scf();
    drawlater();
    contour ( xdata , ydata , rosenbrockC , [2 10 100 500 1000 2000] )
    nmplot_simplexhistory ( nm );
    drawnow();
    demo_viewCode(filename);
    //
    deletefile(simplexfn);
    nm = nmplot_destroy(nm);
    mprintf(_("End of demo.\n"));
endfunction

demo_nmplot_rosen()
clear demo_nmplot_rosen;




