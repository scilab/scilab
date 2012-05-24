// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

//
// Check behaviour with default output function.
//

function [ f , index ] = quadratic ( x , index )
  f = x(1)^2 + x(2)^2;
endfunction
x0 = [1.0 1.0].';
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",quadratic);
nm = neldermead_configure(nm,"-x0",x0);
nm = neldermead_configure(nm,"-outputcommand",neldermead_defaultoutput);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0;0], [], 1e-6 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "maxfuneval" );
nm = neldermead_destroy(nm);

