// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8805 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8805
//
// <-- Short Description -->
// The neldermead function, algorithm box, wrongly used 
// the output function.
// Manually check that the .ref contains the good values for f and x.
//

function [ f , index ] = costf ( x , index )
  f = 100*(x(2) - x(1)^2)^2 + (1 - x(1))^2
endfunction


function stop = myoutputcmd(state, data)
  iter = data.iteration
  if ( state == "init" ) then
    mprintf ( "=================================\n");
    mprintf ( "Initialization\n");
  elseif ( state == "done" ) then
    mprintf ( "=================================\n");
    mprintf ( "End of Optimization\n");
  end
  fc = data.funccount
  fval = data.fval
  x = data.x
  simplex = data.simplex
  // Simplex is a data structure, which can be managed
  // by the optimsimplex class.
  ssize = optimsimplex_size ( simplex )
  if ( modulo(iter,10)==0 ) then
  mprintf ( "Iteration #%d, Feval #%d, Fval = %s, x = %s, Size = %s\n", ..
    iter,fc, string(fval), strcat(string(x)," "), string(ssize));
  end
  stop = %f
endfunction


xopt = [1;1]
fopt = 0
x0 = [-1.2 1.0];
lower_bounds = [-2 -2];
upper_bounds = [2. 2.];
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",costf);
nm = neldermead_configure(nm,"-x0",x0');
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-boundsmin",lower_bounds);
nm = neldermead_configure(nm,"-boundsmax",upper_bounds);
nm = neldermead_configure(nm,"-maxfunevals",400);
nm = neldermead_configure(nm,"-maxiter",250);
nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
nm = neldermead_search(nm);
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal(xopt,[1;1],1.d-1);
nm = neldermead_destroy(nm);



