// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
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

//
// neldermeadBounds.sce --
//   Show a simple neldermead session with bounds.
//

function demo_boxbounds()

    filename = "neldermead_boxbounds.sce";
    dname = get_absolute_file_path(filename);

    mprintf(_("Illustrates Box'' algorithm on a simply bounds-constrained optimization problem.\n"));

    // A simple quadratic function
    function [ f , index ] = myquad ( x , index )
        f = x(1)^2 + x(2)^2
    endfunction
    function y = myquadC ( x1 , x2 )
        y = myquad ( [x1 , x2] , 2 )
    endfunction

    //
    // Initialize the random number generator, so that the results are always the
    // same.
    //
    rand("seed" , 0)

    x0 = [1.2 1.9].';
    // Compute f(x0) : should be close to -2351244.0
    [ fx0 , index ] = myquad ( x0 , 2 );
    mprintf("Computed fx0 = %e (expected = %e)\n",fx0 , 5.05 );

    xopt = [1 1].';
    // Compute f(xopt) : should be 2
    [ fopt , index ] = myquad ( xopt , 2 );
    mprintf("Computed fopt = %e (expected = %e)\n", fopt , 2 );

    nm = nmplot_new ();
    nm = nmplot_configure(nm,"-numberofvariables",2);
    nm = nmplot_configure(nm,"-function",myquad);
    nm = nmplot_configure(nm,"-x0",x0);
    nm = nmplot_configure(nm,"-method","box");
    nm = nmplot_configure(nm,"-boundsmin",[1 1]);
    nm = nmplot_configure(nm,"-boundsmax",[2 2]);

    //
    // Check that the cost function is correctly connected.
    //
    [ nm , f ] = nmplot_function ( nm , x0 );
    simplexfn = fullfile(TMPDIR , "history.simplex.txt")
    nm = nmplot_configure(nm,"-simplexfn",simplexfn);

    //
    // Perform optimization
    //
    mprintf(_("Searching (please wait) ...\n"));
    nm = nmplot_search(nm);
    mprintf(_("...Done\n"));
    //
    // Print a summary
    //
    exec(fullfile(dname,"nmplot_summary.sci"),-1);
    nmplot_summary(nm)
    mprintf("==========================\n");
    xcomp = nmplot_get(nm,"-xopt");
    mprintf("x expected = %s\n",strcat(string(xopt), " "));
    shift = norm(xcomp-xopt)/norm(xopt);
    mprintf("Shift = %f\n",shift);
    fcomp = nmplot_get(nm,"-fopt");
    mprintf("f expected = %f\n",fopt);
    shift = abs(fcomp-fopt)/abs(fopt);
    mprintf("Shift =%f\n",shift);

    //
    // Plot
    //
    mprintf(_("Plot contour (please wait)...\n"));
    xmin = 0.5 ;
    xmax = 2.1 ;
    ymin = 0.5 ;
    ymax = 2.1 ;
    nx = 50 ;
    ny = 50;
    xdata=linspace(xmin,xmax,nx);
    ydata=linspace(ymin,ymax,ny);
    scf();
    xset("fpf"," ")
    drawlater();
    contour ( xdata , ydata , myquadC , 10 )
    nmplot_simplexhistory ( nm );
    drawnow();
    demo_viewCode(filename);
    //
    // Cleanup
    deletefile(simplexfn)
    nm = nmplot_destroy(nm);
    mprintf(_("End of demo.\n"));
endfunction

demo_boxbounds();
clear demo_boxbounds;






