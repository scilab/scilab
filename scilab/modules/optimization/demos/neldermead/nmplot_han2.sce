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


function demo_nmplot_2()
    filename = "nmplot_han2.sce";
    dname = get_absolute_file_path(filename);

    mprintf(_("Illustrates the 2nd counter example given by Han et al.\n"));

    //
    // han2 --
    //   Compute the cost function from the Han Phd Thesis
    //   which exhibits a failure of the NM method.
    // Reference
    //   Algorithms in Unconstrained Optimization
    //   Han, Lixing
    //   2000
    //   Ph.D., The University of Connecticut
    //
    function [ f , index ] = han2 ( x , index )
        if abs(x(2)) <= 1.0 then
            rho = 0.0
        elseif x(2) > 1.0 then
            rho = x(2) - 1
        else
            rho = -x(2) - 1
        end
        f = x(1)^2 + rho;
    endfunction
    function y = han2C ( x1 , x2 )
        y = han2 ( [x1 , x2] , 2 )
    endfunction


    coords0 = [
    0.    0.5
    0.   -0.5
    1.    0.];

    mprintf(_("Creating nmplot object ...\n"));
    nm = nmplot_new ();
    nm = nmplot_configure(nm, "-numberofvariables", 2);
    nm = nmplot_configure(nm, "-function", han2);
    nm = nmplot_configure(nm, "-x0", [1.0 1.0]');
    nm = nmplot_configure(nm, "-maxiter", 50);
    nm = nmplot_configure(nm, "-maxfunevals", 300);
    nm = nmplot_configure(nm, "-tolfunrelative", 10*%eps);
    nm = nmplot_configure(nm, "-tolxrelative", 10*%eps);
    nm = nmplot_configure(nm, "-simplex0method", "given");
    nm = nmplot_configure(nm, "-coords0", coords0);

    //
    // Setup output files
    //
    simplexfn = TMPDIR + filesep() + "history.simplex.txt";
    nm = nmplot_configure(nm, "-simplexfn", simplexfn);

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
    // Plot
    //
    mprintf(_("Plotting contour (please wait) ...\n"));
    xmin = -0.2 ;
    xmax = 1.2 ;
    ymin = -1.5 ;
    ymax = 1.5 ;
    nx = 50 ;
    ny = 50;
    scf();
    xset("fpf"," ")
    drawlater();
    xdata=linspace(xmin,xmax,nx);
    ydata=linspace(ymin,ymax,ny);
    contour ( xdata , ydata , han2C , [0.1 0.2 0.5 1.0 1.5 1.9] )
    nmplot_simplexhistory ( nm );
    drawnow();
    demo_viewCode(filename);
    //
    // Cleanup
    deletefile(simplexfn);
    nm = nmplot_destroy(nm);
    mprintf(_("End of demo.\n"));
endfunction

demo_nmplot_2()
clear demo_nmplot_2;






