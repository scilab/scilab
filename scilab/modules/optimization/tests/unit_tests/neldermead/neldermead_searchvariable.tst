// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->


function [ y , index ] = rosenbrock ( x , index )
    y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test on Rosenbrock test case
//

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-storehistory",%t);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [1.0;1.0], 1e-6 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0 , [] , 1e-6 );
// Check function value history
historyfopt = neldermead_get(nm,"-historyfopt");
iterations = neldermead_get(nm,"-iterations");
sizefopt = size(historyfopt);
assert_checkequal ( [iterations 1], sizefopt );
// Check number of function evaluations
funevals = neldermead_get(nm,"-funevals");
assert_checkequal ( (funevals < 300) , %T )
// Check optimum history
historyxopt = neldermead_get(nm,"-historyxopt");
sizexopt = size(historyxopt);
assert_checkequal ( iterations, sizexopt );
// Check simplex history
historysimplex = neldermead_get(nm,"-historysimplex");
sizexsimplex = size(historysimplex);
assert_checkequal ( [iterations 3 2], sizexsimplex );
// Check fx0
fx0 = neldermead_get(nm,"-fx0");
assert_checkalmostequal ( fx0 , 24.2 , 10*%eps );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolsize" );
// Cleanup
nm = neldermead_destroy(nm);

// Check that the verbose mode is functional
// Few iterations are necessary to check this
// Many iterations costs a lot more in time.
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",5);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-storehistory",%t);
nm = neldermead_search(nm, "off");
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "maxiter" );
nm = neldermead_destroy(nm);

