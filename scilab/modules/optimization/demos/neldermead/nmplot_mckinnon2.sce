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


function demo_mckinnon2()

    mprintf(_("Defining McKinnon function...\n"));

    //% MCKINNON computes the McKinnon function.
    //
    //  Discussion:
    //
    //    This function has a global minimizer:
    //
    //      X* = ( 0.0, -0.5 ), F(X*) = -0.25
    //
    //    There are three parameters, TAU, THETA and PHI.
    //
    //    1 < TAU, then F is strictly convex.
    //             and F has continuous first derivatives.
    //    2 < TAU, then F has continuous second derivatives.
    //    3 < TAU, then F has continuous third derivatives.
    //
    //    However, this function can cause the Nelder-Mead optimization
    //    algorithm to "converge" to a point which is not the minimizer
    //    of the function F.
    //
    //    Sample parameter values which cause problems for Nelder-Mead
    //    include:
    //
    //      TAU = 1, THETA = 15, PHI =  10;
    //      TAU = 2, THETA =  6, PHI =  60;
    //      TAU = 3, THETA =  6, PHI = 400;
    //
    //    To get the bad behavior, we also assume the initial simplex has the form
    //
    //      X1 = (0,0),
    //      X2 = (1,1),
    //      X3 = (A,B),
    //
    //    where
    //
    //      A = (1+sqrt(33))/8 =  0.84307...
    //      B = (1-sqrt(33))/8 = -0.59307...
    //
    //  Licensing:
    //
    //    This code is distributed under the GNU LGPL license.
    //
    //  Modified:
    //
    //    09 February 2008
    //
    //  Author:
    //
    //    John Burkardt
    //
    //  Reference:
    //
    //    Ken McKinnon,
    //    Convergence of the Nelder-Mead simplex method to a nonstationary point,
    //    SIAM Journal on Optimization,
    //    Volume 9, Number 1, 1998, pages 148-158.
    //
    //  Parameters:
    //
    //    Input, real X(2), the argument of the function.
    //
    //    Output, real F, the value of the function at X.
    //
    // Copyright (C) 2009 - INRIA - Michael Baudin, Scilab port

    function [ f , index ] = mckinnon3 ( x , index )

        if ( length ( x ) ~= 2 )
            error (_("Error: function expects a two dimensional input\n"));
        end

        tau = 3.0;
        theta = 6.0;
        phi = 400.0;

        if ( x(1) <= 0.0 )
            f = theta * phi * abs ( x(1) ).^tau + x(2) * ( 1.0 + x(2) );
        else
            f = theta       *       x(1).^tau   + x(2) * ( 1.0 + x(2) );
        end
    endfunction
    function y = mckinnon3C ( x1 , x2 )
        y = mckinnon3 ( [x1 , x2] , 2 )
    endfunction

    lambda1 = (1.0 + sqrt(33.0))/8.0;
    lambda2 = (1.0 - sqrt(33.0))/8.0;
    coords0 = [
    1.0  1.0
    0.0  0.0
    lambda1 lambda2
    ];


    x0 = [1.0 1.0]';
    mprintf(_("x0=%s\n"), strcat(string(x0)," "));
    mprintf(_("Creating object...\n"));
    nm = nmplot_new ();
    nm = nmplot_configure(nm, "-numberofvariables",2);
    nm = nmplot_configure(nm, "-function",mckinnon3);
    nm = nmplot_configure(nm, "-x0",x0);
    nm = nmplot_configure(nm, "-maxiter",200);
    nm = nmplot_configure(nm, "-maxfunevals",300);
    nm = nmplot_configure(nm, "-tolsimplexizerelative",1.e-6);
    nm = nmplot_configure(nm, "-simplex0method","given");
    nm = nmplot_configure(nm, "-coords0",coords0);
    nm = nmplot_configure(nm, "-kelleystagnationflag",%t);
    nm = nmplot_configure(nm, "-restartflag",%t);
    nm = nmplot_configure(nm, "-restartdetection","kelley");
    //
    // Setup output files
    //
    simplexfn = TMPDIR + filesep() + "history.simplex.txt";
    fbarfn = TMPDIR + filesep() + "history.fbar.txt";
    foptfn = TMPDIR + filesep() + "history.fopt.txt";
    sigmafn = TMPDIR + filesep() + "history.sigma.txt";
    nm = nmplot_configure(nm, "-simplexfn",simplexfn);
    nm = nmplot_configure(nm, "-fbarfn",fbarfn);
    nm = nmplot_configure(nm, "-foptfn",foptfn);
    nm = nmplot_configure(nm, "-sigmafn",sigmafn);
    //
    // Perform optimization
    //
    mprintf(_("Searching (please wait) ...\n"));
    nm = nmplot_search(nm);
    disp(nm);

    //
    // Plot
    //
    mprintf(_("Plot contour (please wait) ...\n"));
    xmin = -0.2;
    xmax = 1.2 ;
    ymin = -2.0 ;
    ymax = 2.0 ;
    nx = 50 ;
    ny = 50;
    xdata=linspace(xmin,xmax,nx);
    ydata=linspace(ymin,ymax,ny);
    scf();
    f = gcf();
    f.axes_size = [710, 560];
    subplot(2,2,1)
    xset("fpf"," ")
    drawlater();
    contour ( xdata , ydata , mckinnon3C , [-0.2 0.0 1.0 2.0 5.0 10.0 20.0] )
    nmplot_simplexhistory ( nm );
    drawnow();
    subplot(2,2,2)
    mytitle = _("Function Value Average");
    myxlabel = _("Iterations");
    nmplot_historyplot ( nm , fbarfn, mytitle , myxlabel );
    subplot(2,2,3)
    mytitle = _("Minimum Function Value") ;
    myxlabel = _("Iterations");
    nmplot_historyplot ( nm , foptfn, mytitle , myxlabel );
    subplot(2,2,4)
    mytitle = _("Maximum Oriented length") ;
    myxlabel = _("Iterations") ;
    nmplot_historyplot ( nm , sigmafn, mytitle , myxlabel );
    demo_viewCode("nmplot_mckinnon2.sce");
    deletefile(simplexfn);
    deletefile(fbarfn);
    deletefile(foptfn);
    deletefile(sigmafn);
    nm = nmplot_destroy(nm);
    mprintf(_("End of demo.\n"));
endfunction

demo_mckinnon2();
clear demo_mckinnon2



