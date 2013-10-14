// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->




function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.0 2.0]');
nm = neldermead_configure(nm,"-simplex0method","spendley");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_search(nm, "off");
simplex = neldermead_get(nm,"-simplex0");
computed = optimsimplex_getallx( simplex );
expected = [
1.    2.
1.96592582628907    2.25881904510252
1.25881904510252  2.96592582628907
];
assert_checkalmostequal ( computed, expected, 10 * %eps );
nm = neldermead_destroy(nm);
clear nm;

