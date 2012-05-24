// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- CLI SHELL MODE -->

function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

// Forward
s1 = optimsimplex_new ();
simplex = [
1. -2.0 1.0
4. -1.0 3.0
7. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
computed = optimsimplex_gradientfv ( s1 );
assert_checkalmostequal ( computed , [-3.0 3.0]' , 10 * %eps );
s1 = optimsimplex_destroy ( s1 );
// Centered
s1 = optimsimplex_new ();
simplex = [
1. -2.0 1.0
4. -1.0 3.0
7. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
computed = optimsimplex_gradientfv ( s1 , method = "centered" , fun = rosenbrock );
assert_checkalmostequal ( computed , [-1636.3333333333333 -1684.8333333333333]' , 10 * %eps );
s1 = optimsimplex_destroy ( s1 );
// Centered with additionnal arguments
myobj = tlist(["T_MYSTUFF","nb"]);
myobj.nb = 0;
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction
s1 = optimsimplex_new ();
simplex = [
1. -2.0 1.0
4. -1.0 3.0
7. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
[ computed , myobj ] = optimsimplex_gradientfv ( s1 , method = "centered" , fun = mycostf , data = myobj );
assert_checkalmostequal ( computed , [-1636.3333333333333 -1684.8333333333333]' , 10 * %eps );
assert_checkequal ( myobj.nb , 2 );
s1 = optimsimplex_destroy ( s1 );

