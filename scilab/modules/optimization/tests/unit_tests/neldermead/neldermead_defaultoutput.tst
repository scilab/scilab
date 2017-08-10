// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
nm = neldermead_search(nm, "off");
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0;0], [], 1e-6 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "maxfuneval" );
nm = neldermead_destroy(nm);

