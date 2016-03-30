// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
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

//
// nmplot_boxproblemA.sce --
//   Show that the Box algorithm is able to reproduce the
//   numerical experiment presented in Box's paper.
//

function demo_boxproblemA()

    filename = "neldermead_boxproblemA.sce";
    dname = get_absolute_file_path(filename);

    mprintf(_("Illustrates Box'' algorithm on Box problem A.\n"));

    mprintf("M.J. Box, \n");
    mprintf(_("""A new method of constrained optimization \n"));
    mprintf(_("and a comparison with other methods"".\n"));
    mprintf("The Computer Journal, Volume 8, Number 1, 1965, 42--52\n");
    mprintf(_("Problem A\n"));


    //
    //  Reference:
    //
    //   M.J. Box,
    //   "A new method of constrained optimization
    //   and a comparison with other methods".
    //   The Computer Journal, Volume 8, Number 1, 1965, 42--52
    //   Problem A
    //
    //   Algorithm 454: the complex method for constrained optimization [E4]
    //   Communications of the ACM, Volume 16 ,  Issue 8  (August 1973)
    //   Pages: 487 - 489
    //

    //
    // boxproblemA --
    //   Computes the Box problem A cost function and
    //   inequality constraints.
    //
    // Arguments
    //   x: the point where to compute the function
    //   index : the stuff to compute
    //   data : the parameters of Box cost function
    //
    function [ f , c , index ] = boxproblemA ( x , index , data )
        f = []
        c = []
        b = x(2) + 0.01 * x(3)
        x6 = (data.k1 + data.k2 * x(2) ...
        + data.k3 * x(3) + data.k4 * x(4) + data.k5 * x(5)) * x(1)
        y1 = data.k6 + data.k7 * x(2) + data.k8 * x(3) ...
        + data.k9 * x(4) + data.k10 * x(5)
        y2 = data.k11 + data.k12 * x(2) + data.k13 * x(3) ...
        + data.k14 * x(4) + data.k15 * x(5)
        y3 = data.k16 + data.k17 * x(2) + data.k18 * x(3) ...
        + data.k19 * x(4) + data.k20 * x(5)
        y4 = data.k21 + data.k22 * x(2) + data.k23 * x(3) ...
        + data.k24 * x(4) + data.k25 * x(5)
        x7 = ( y1 + y2 + y3 ) * x(1)
        x8 = (data.k26 + data.k27 * x(2) + data.k28 * x(3) ...
        + data.k29 * x(4) ...
        + data.k30 * x(5) ) * x(1) + x6 + x7

        if ( index == 2 | index == 6 ) then
            f = (data.a2 * y1 + data.a3 * y2 + data.a4 * y3 + data.a5 * y4 ...
            + 7840 * data.a6 - 100000 * data.a0 ...
            - 50800 * b * data.a7 + data.k31 + data.k32 * x(2) + data.k33 * x(3) ...
            + data.k34 * x(4) + data.k35 * x(5)) * x(1) ...
            - 24345 + data.a1 * x6
            f = -f
        end

        if ( index == 5 | index == 6 ) then
            c1 = x6
            c2 = 294000 - x6
            c3 = x7
            c4 = 294000 - x7
            c5 = x8
            c6 = 277200 - x8
            c = [c1 c2 c3 c4 c5 c6]
        end
    endfunction

    boxparams = struct();
    boxparams.a0 = 9;
    boxparams.a1 = 15;
    boxparams.a2 = 50;
    boxparams.a3 = 9.583;
    boxparams.a4 = 20;
    boxparams.a5 = 15;
    boxparams.a6 = 6;
    boxparams.a7 = 0.75;
    boxparams.k1 = -145421.402;
    boxparams.k2 = 2931.1506;
    boxparams.k3 = -40.427932;
    boxparams.k4 = 5106.192;
    boxparams.k5 = 15711.36;
    boxparams.k6 = -161622.577;
    boxparams.k7 = 4176.15328;
    boxparams.k8 = 2.8260078;
    boxparams.k9 = 9200.476;
    boxparams.k10 = 13160.295;
    boxparams.k11 = -21686.9194;
    boxparams.k12 = 123.56928;
    boxparams.k13 = -21.1188894;
    boxparams.k14 = 706.834;
    boxparams.k15 = 2898.573;
    boxparams.k16 = 28298.388;
    boxparams.k17 = 60.81096;
    boxparams.k18 = 31.242116;
    boxparams.k19 = 329.574;
    boxparams.k20 = -2882.082;
    boxparams.k21 = 74095.3845;
    boxparams.k22 = -306.262544;
    boxparams.k23 = 16.243649;
    boxparams.k24 = -3094.252;
    boxparams.k25 = -5566.2628;
    boxparams.k26 = -26237.0;
    boxparams.k27 = 99.0;
    boxparams.k28 = -0.42;
    boxparams.k29 = 1300.0;
    boxparams.k30 = 2100.0;
    boxparams.k31 = 925548.252;
    boxparams.k32 = -61968.8432;
    boxparams.k33 = 23.3088196;
    boxparams.k34 = -27097.648;
    boxparams.k35 = -50843.766;

    //
    // Initialize the random number generator, so that the results are always the
    // same.
    //
    rand("seed" , 0)

    x0 = [2.52 2.0 37.5 9.25 6.8].';
    // Compute f(x0) : should be close to -2351244.0
    [ fx0 , c , index ] = boxproblemA ( x0 , 2 , boxparams );
    mprintf("Computed fx0 = %e (expected = %e)\n",fx0 , -2351244. );

    xopt = [4.53743 2.4 60.0 9.3 7.0].';
    // Compute f(xopt) : should be -5280334.0
    [ fopt , c , index ] = boxproblemA ( xopt , 2 , boxparams );
    mprintf("Computed fopt = %e (expected = %e)\n", fopt , -5280334.0 );

    nm = neldermead_new ();
    nm = neldermead_configure(nm,"-numberofvariables",5);
    nm = neldermead_configure(nm,"-function",list(boxproblemA,boxparams));
    nm = neldermead_configure(nm,"-x0",x0);
    nm = neldermead_configure(nm,"-maxiter",300);
    nm = neldermead_configure(nm,"-maxfunevals",1000);
    nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-4);
    nm = neldermead_configure(nm,"-method","box");

    // Configure like Box
    nm = neldermead_configure(nm,"-boundsmin",[0.0 1.2 20.0 9.0 6.0]);
    nm = neldermead_configure(nm,"-boundsmax",[5.0 2.5 60.0 9.3 7.0]);
    nm = neldermead_configure(nm,"-simplex0method","randbounds");
    nm = neldermead_configure(nm,"-nbineqconst",6);

    //
    // Check that the cost function is correctly connected.
    //
    [ nm , f ] = neldermead_function ( nm , x0 );

    //
    // Perform optimization
    //
    mprintf(_("Searching (please wait)...\n"));
    nm = neldermead_search(nm, "off");
    mprintf(_("...Done\n"));
    //
    // Print a summary
    //
    exec(fullfile(dname,"neldermead_summary.sci"),-1);
    neldermead_summary(nm)
    mprintf("==========================\n");
    xcomp = neldermead_get(nm,"-xopt");
    mprintf("x expected = [%s]\n",strcat(string(xopt)," "));
    shift = norm(xcomp-xopt)/norm(xopt);
    mprintf(_("Relative error on x = %e\n"),shift);
    fcomp = neldermead_get(nm,"-fopt");
    mprintf("f expected = %f\n",fopt);
    shift = abs(fcomp-fopt)/abs(fopt);
    mprintf("Relative error on f = %e\n",shift);
    nm = neldermead_destroy(nm);
    mprintf(_("End of demo.\n"));

    //
    // Load this script into the editor
    //
    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if(m == 1)
        editor ( dname + filename, "readonly" );
    end
endfunction

demo_boxproblemA();
clear demo_boxproblemA;








