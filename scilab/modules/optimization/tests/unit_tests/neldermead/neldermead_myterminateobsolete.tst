// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

// This test is designed to produce a warning:
// this warning is localized.
// This is why we do not check the ref.
// Checking the ref file under Linux fails, because
// <-- ENGLISH IMPOSED --> has no effect there.
// See : http://bugzilla.scilab.org/show_bug.cgi?id=9284


function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Check backward compatibility:
// check obsolete options "-myterminateflag" and "-myterminate".
//
function [ this , terminate , status ] = mystoppingrule2 ( this , simplex )
  ssize = optimsimplex_size ( simplex , "sigmaplus" );
  if ( ssize < 1.e-2 ) then
    terminate = %t;
    status = "mysize";
  else
    terminate = %f
  end

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
nm = neldermead_configure(nm,"-myterminateflag",%t);
nm = neldermead_configure(nm,"-myterminate",mystoppingrule2);
//
// Check cget
value = neldermead_cget(nm,"-myterminateflag");
assert_checktrue ( value );
//
value = neldermead_cget(nm,"-myterminate");
assert_checkequal ( typeof(value) , "function" );
//
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [1.0 1.0]', 1e-2 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0 , [] , 1e-4 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "mysize" );
nm = neldermead_destroy(nm);

