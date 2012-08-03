// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9566 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9566
//
// <-- Short Description -->
// The tolvarianceflag, tolabsolutevariance, tolrelativevariance options of
// neldermead are unnecessary.
//

function stop = myoutputcmd ( state, data, tolrelativevariance, tolabsolutevariance, variancesimplex0 )
    simplex = data.simplex
    stop = %f
    if ( state == "iter") then
        var = optimsimplex_fvvariance ( simplex )
        if ( var < tolrelativevariance * variancesimplex0 + tolabsolutevariance ) then
            stop = %t;
        end
    end
endfunction

function [ y , index ] = rosenbrock ( x , index )
    y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test with my own termination criteria 
//
x0 = [1.1 1.1]';
simplex0 = optimsimplex_new ( "axes" , x0.' );
coords0 = optimsimplex_getallx(simplex0);
variancesimplex0 = optimsimplex_fvvariance ( simplex0 );
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
// Disable default terminations
nm = neldermead_configure(nm,"-maxiter",%inf);
nm = neldermead_configure(nm,"-maxfunevals",%inf);
nm = neldermead_configure(nm,"-tolxmethod",%f);
nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
tolabsolutevariance = 1.e-6;
tolrelativevariance = 1.e-6;
stopfun = list(myoutputcmd, tolrelativevariance, tolabsolutevariance, variancesimplex0);
nm = neldermead_configure(nm,"-outputcommand",stopfun);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [1.0 1.0]', 1e-2 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0 , [] , 1e-3 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "userstop" );
// Check simplex variance
simplex = neldermead_get(nm,"-simplexopt");
var = optimsimplex_fvvariance ( simplex );
ssize = optimsimplex_size ( simplex , "sigmaplus" );
assert_checktrue ( var < tolrelativevariance * variancesimplex0 + tolabsolutevariance );
// Check function evaluations
funevals = neldermead_get(nm,"-funevals");
assert_checkequal ( funevals<200 , %t );
nm = neldermead_destroy(nm);
