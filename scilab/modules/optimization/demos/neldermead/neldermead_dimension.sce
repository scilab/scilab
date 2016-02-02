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


function demo_dimension()

    mprintf(_("Illustrates the sensitivity to dimension of the Nelder-Mead algorithm\n"));
    mprintf(_("Han and Neumann. ""Effect of dimensionality on the nelder-mead simplex method."" Optimization Methods and Software, 2006.\n"));

    function [ f , index ] = quadracticn ( x , index )
        f = sum(x.^2);
    endfunction

    //
    // solvepb --
    //   Find the solution for the given number of dimensions
    //
    function [nbfevals , niter , rho] = solvepb ( n )
        rand("seed", 0)
        nm = neldermead_new ();
        nm = neldermead_configure(nm, "-numberofvariables", n);
        nm = neldermead_configure(nm, "-function", quadracticn);
        nm = neldermead_configure(nm, "-x0", zeros(n,1));
        nm = neldermead_configure(nm, "-maxiter", 2000);
        nm = neldermead_configure(nm, "-maxfunevals", 2000);
        nm = neldermead_configure(nm, "-tolxmethod", %f);
        nm = neldermead_configure(nm, "-tolsimplexizerelative", 0.0);
        nm = neldermead_configure(nm, "-tolsimplexizeabsolute", 1.e-8);
        nm = neldermead_configure(nm, "-simplex0method", "given");
        coords (1,1:n) = zeros(1,n);
        for i = 2:n+1
            coords (i,1:n) = 2.0 * rand(1,n) - 1.0;
        end
        nm = neldermead_configure(nm, "-coords0", coords);
        //
        // Perform optimization
        //
        nm = neldermead_search(nm);
        si0 = neldermead_get ( nm , "-simplex0" );
        sigma0 = optimsimplex_size ( si0 , "sigmaplus" );
        siopt = neldermead_get ( nm , "-simplexopt" );
        sigmaopt = optimsimplex_size ( siopt , "sigmaplus" );
        niter = neldermead_get ( nm , "-iterations" );
        rho = (sigmaopt/sigma0)^(1.0/niter);
        nbfevals = neldermead_get ( nm , "-funevals" );
        mprintf ( "%d %d %d %f\n", n , nbfevals , niter , rho );
        nm = neldermead_destroy(nm);
    endfunction

    nmax = 20;
    mprintf(_("Maximum dimension:%d\n"),nmax);
    mprintf(_("Column #1: number of dimensions\n"));
    mprintf(_("Column #2: number of function evaluations\n"));
    mprintf(_("Column #3: number of iterations\n"));
    mprintf(_("Column #4: convergence rate (lower is better)\n"));

    for n = 1:nmax
        [nbfevals niter rho] = solvepb ( n );
        array_rho(n) = rho;
        array_nbfevals(n) = nbfevals;
        array_niter(n) = niter;
    end

    // Plot rate of convergence
    hh = scf();
    plot(1:nmax,array_rho)
    hh.children.x_label.text = _("Number of parameters")
    hh.children.y_label.text = _("Rate of convergence")
    hh.children.children.children.mark_mode = "on";
    hh.children.children.children.mark_style = 9;
    hh.children.children.children.mark_size = 10;

    // Plot number of function evaluations
    hh = scf();
    plot(1:nmax,array_nbfevals)
    hh.children.x_label.text = _("Number of parameters")
    hh.children.y_label.text = _("Number of function evaluations")
    hh.children.children.children.mark_mode = "on";
    hh.children.children.children.mark_style = 9;
    hh.children.children.children.mark_size = 10;
    mprintf(_("End of demo.\n"));

    //
    //Load this script into the editor
    //

    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if(m == 1)
        filename = "neldermead_dimension.sce";
        dname = get_absolute_file_path(filename);
        editor ( dname + filename, "readonly" );
    end
endfunction

demo_dimension();
clear demo_dimension;








