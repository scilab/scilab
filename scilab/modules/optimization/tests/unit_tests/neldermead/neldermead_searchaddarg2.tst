// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->



//
// In this case, the mydata variable is passed
// explicitely by the neldermead class.
// So the actual name "mydata" does not matter
// and whatever variable name can be used.
//
function [ y , index , mydata ] = rosenbrock ( x , index , mydata )
  a = mydata.a
  y = 100*(x(2)-x(1)^2)^2 + ( a - x(1))^2;
endfunction

//
// Test with an additional argument
//
mystuff = tlist(["T_MYSTUFF","a"]);
mystuff.a = 12.0;

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",400);
nm = neldermead_configure(nm,"-maxfunevals",800);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-storehistory",%t);
nm = neldermead_configure(nm,"-costfargument",mystuff);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [12.0 144.0]', 1e-6 );
nm = neldermead_destroy(nm);


