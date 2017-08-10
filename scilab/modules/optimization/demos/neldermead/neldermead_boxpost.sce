// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
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
// nmplot_boxpost.sce --
//   Show that the Box algorithm is able to reproduce the
//   numerical experiment presented in Richardson and Kuester's paper.
//   Rosenbrock's Post Office
//

function demo_boxpost()

    filename = "neldermead_boxpost.sce";
    dname = get_absolute_file_path(filename);

    mprintf(_("Illustrates Box'' algorithm on Rosenbrock''s Post Office Problem.\n"));
    mprintf(_("Defining Rosenbrock Post Office function...\n"));

    //
    //  Reference:
    //
    //    Algorithm 454
    //    The complex method for constrained
    //    optimization
    //    Richardson, Kuester
    //    1971
    //
    //    An automatic method for finding the
    //    greatest or least value of a function
    //    Rosenbrock
    //    1960
    //
    //   Richardson and Kuester Results :
    //   F=3456
    //   X1 = 24.01
    //   X2 = 12.00
    //   X3 = 12.00
    //   Iterations : 72
    //
    //

    //
    // fpostoffice --
    //   Computes the Post Office cost function and
    //   inequality constraints.
    //
    // Arguments
    //   x: the point where to compute the function
    //   index : the stuff to compute
    //

    function [ f , c , index ] = fpostoffice ( x , index )
        f = []
        c = []
        if ( index==2 | index==6 ) then
            f = -x(1) * x(2) * x(3)
        end

        if ( index==5 | index==6 ) then
            c1 = x(1) + 2 * x(2) + 2 * x(3)
            c2 = 72 - c1
            c = [c1 c2]
        end
    endfunction
    //
    // Initialize the random number generator, so that the results are always the
    // same.
    //
    rand("seed" , 0)

    x0 = [1.0 1.0 1.0].';
    // Compute f(x0) : should be close to -1
    fx0 = fpostoffice ( x0 , 2 );
    mprintf("Computed fx0 = %e (expected = %e)\n",fx0 , -1 );
    [ fx0 , cx0, index ] = fpostoffice ( x0 , 6 );
    mprintf("Computed Constraints(x0) = [%e %e]\n", ..
    cx0(1), cx0(2) );
    mprintf("Expected Constraints(x0) = [%e %e]\n", ..
    5 , 67 );

    xopt = [24 12 12].';
    // Compute f(xopt) : should be 3456
    fopt = fpostoffice ( xopt );
    mprintf("Computed fopt = %e (expected = %e)\n", fopt , -3456 );

    nm = neldermead_new ();
    nm = neldermead_configure(nm,"-numberofvariables",3);
    nm = neldermead_configure(nm,"-function",fpostoffice);
    nm = neldermead_configure(nm,"-x0",x0);
    nm = neldermead_configure(nm,"-maxiter",300);
    nm = neldermead_configure(nm,"-maxfunevals",500);
    nm = neldermead_configure(nm,"-method","box");
    nm = neldermead_configure(nm,"-verbose",1);
    logfile = TMPDIR + "/postoffice.txt";
    nm = neldermead_configure(nm,"-logfile" , logfile );
    nm = neldermead_configure(nm,"-verbosetermination",1);
    nm = neldermead_configure(nm,"-boundsmin",[0.0 0.0 0.0]);
    nm = neldermead_configure(nm,"-boundsmax",[42.0 42.0 42.0]);
    // Configure like Box
    nm = neldermead_configure(nm,"-simplex0method","randbounds");
    nm = neldermead_configure(nm,"-nbineqconst",2);
    nm = neldermead_configure(nm,"-tolxmethod" , %f );
    nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
    nm = neldermead_configure(nm,"-boxtermination" , %t );
    nm = neldermead_configure(nm,"-boxtolf" , 0.001 );
    nm = neldermead_configure(nm,"-boxboundsalpha" , 0.0001 );

    //
    // Check that the cost function is correctly connected.
    //
    [ nm , result ] = neldermead_function ( nm , x0 );

    //
    // Perform optimization
    //
    mprintf(_("Searching (please wait) ...\n"));
    nm = neldermead_search(nm);
    //
    // Print a summary
    //
    exec(fullfile(dname,"neldermead_summary.sci"),-1);
    neldermead_summary(nm)
    mprintf("==========================\n");
    xcomp = neldermead_get(nm,"-xopt");
    mprintf("x expected = [%s]\n",strcat(string(xopt)," "));
    shift = norm(xcomp-xopt)/norm(xopt);
    mprintf("Shift = %f\n",shift);
    fcomp = neldermead_get(nm,"-fopt");
    mprintf("f expected = %f\n",fopt);
    shift = abs(fcomp-fopt)/abs(fopt);
    mprintf("Shift = %f\n",shift);
    nm = neldermead_destroy(nm);
    deletefile ( logfile )
    mprintf(_("End of demo.\n"));

    //
    // Load this script into the editor
    //
    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if(m == 1)
        editor ( dname + filename, "readonly" );
    end
endfunction

demo_boxpost();
clear demo_boxpost;










