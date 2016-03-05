// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
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


function demo_outputcmd()

    function [ y , index ] = rosenbrock ( x , index )
        y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
    endfunction


    //
    // myoutputcmd --
    //  This command is called back by the Nelder-Mead
    //  algorithm.
    // Arguments
    //  state : the current state of the algorithm
    //    "init", "iter", "done"
    //  data : the data at the current state
    //    This is a tlist with the following entries:
    //    * x : the optimal vector of parameters
    //    * fval : the minimum function value
    //    * simplex : the simplex, as a simplex object
    //    * iteration : the number of iterations performed
    //    * funccount : the number of function evaluations
    // stop : set to %t to interrupt the algorithm
    //

    function stop = myoutputcmd ( state , data )
        iter = data.iteration
        if ( state == "init" ) then
            mprintf ( "=================================\n");
            mprintf ( _("Initialization\n"));
        elseif ( state == "done" ) then
            mprintf ( "=================================\n");
            mprintf ( _("End of Optimization\n"));
        end
        fc = data.funccount
        fval = data.fval
        x = data.x
        simplex = data.simplex
        // Simplex is a data structure, which can be managed
        // by the optimsimplex class.
        ssize = optimsimplex_size ( simplex )
        mprintf ( "Iteration #%d, Feval #%d, Fval = %e, x = %s, Size = %e\n", iter, fc, fval, strcat(string(x)," "), ssize);
        stop = %f
    endfunction


    nm = neldermead_new ();
    nm = neldermead_configure(nm,"-numberofvariables",2);
    nm = neldermead_configure(nm,"-function",rosenbrock);
    nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
    nm = neldermead_configure(nm,"-maxiter",200);
    nm = neldermead_configure(nm,"-maxfunevals",350);
    nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
    nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
    nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
    nm = neldermead_search(nm);
    nm = neldermead_destroy(nm);
    mprintf(_("End of demo.\n"));

    //
    // Load this script into the editor
    //
    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if(m == 1)
        filename = "neldermead_outputcmd.sce";
        dname = get_absolute_file_path(filename);
        editor ( dname + filename, "readonly" );
    end
endfunction

demo_outputcmd()
clear demo_outputcmd;







