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




function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test optimsimplex_reflect
//

//
// Test with 3 vertices
//
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
r = optimsimplex_reflect ( s1 , rosenbrock );
computed = optimsimplex_getall ( r );
expected = [
24.2 -1.2 1.0
1484.8 -2.2 1.0
212.2 -1.2 0.0
];
assert_checkalmostequal ( computed , expected , 10 * %eps );
s1 = optimsimplex_destroy ( s1 );
r = optimsimplex_destroy ( r );
//
// Test with an additional argument
//
myobj = tlist(["T_MYSTUFF","nb"]);
myobj.nb = 0;
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
[ r , myobj ] = optimsimplex_reflect ( s1 , mycostf , myobj );
computed = optimsimplex_getall ( r );
expected = [
24.2 -1.2 1.0
1484.8 -2.2 1.0
212.2 -1.2 0.0
];
assert_checkalmostequal ( computed , expected , 10 * %eps );
assert_checkequal ( myobj.nb , 2 );
s1 = optimsimplex_destroy ( s1 );
r = optimsimplex_destroy ( r );
//
// Test with 5 vertices
//
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
12.0 -2.0 -2.0
24.0 -4.0 4.0
];
s1 = optimsimplex_setall ( s1 , simplex );
r = optimsimplex_reflect ( s1 , rosenbrock );
computed = optimsimplex_getall ( r );
expected = [
    24.2     -1.2   1.  
    1484.8   -2.2   1.  
    212.2    -1.2   0.  
    1476.52  -0.4   4.  
    2079.72   1.6  -2.  
];
assert_checkalmostequal ( computed , expected , 10 * %eps );
s1 = optimsimplex_destroy ( s1 );
r = optimsimplex_destroy ( r );

