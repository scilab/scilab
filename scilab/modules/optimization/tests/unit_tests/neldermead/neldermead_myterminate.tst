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





function stop = myoutputcmd ( state , data )
  simplex = data.simplex
  ssize = optimsimplex_size ( simplex , "sigmaplus" );
  if ( ssize < 1.e-2 ) then
    stop = %t;
    status = "mysize";
  else
    stop = %f
  end
endfunction

function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction




//
// Test with my own termination criteria 
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-maxiter",%inf);
nm = neldermead_configure(nm,"-maxfunevals",%inf);
nm = neldermead_configure(nm,"-method","variable");
// Disable default terminations
nm = neldermead_configure(nm,"-tolxmethod",%f);
nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [1.0 1.0]', 1e-2 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0 , [] , 1e-4 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "userstop" );
// Check simplex size
simplex = neldermead_get(nm,"-simplexopt");
ssize = optimsimplex_size ( simplex , "sigmaplus" );
assert_checkequal ( ssize<1.e-1 , %t );
// Check function evaluations
funevals = neldermead_get(nm,"-funevals");
assert_checkequal ( funevals<200 , %t );
nm = neldermead_destroy(nm);


