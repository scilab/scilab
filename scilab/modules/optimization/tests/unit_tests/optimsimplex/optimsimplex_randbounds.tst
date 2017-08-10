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
// Test randbounds with default number of vertices
//
rand("seed" , 0)
s1 = optimsimplex_new ( "randbounds" , [-1.2 1.0], rosenbrock, ...
[-5.0 -5.0] , [5.0 5.0] );
computed = optimsimplex_getall ( s1 );
expected = [
24.19999999999999573674   -1.1999999999999999555911    1.
3347.7382596240795464837  -2.8867513453587889671326    2.5604385416954755783081
71189.511402687028748915  -4.9977886537089943885803  -1.69672908261418342590
]
assert_checkalmostequal ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test randbounds with 5 vertices
//
rand("seed" , 0)
s1 = optimsimplex_new ( "randbounds" , [-1.2 1.0], rosenbrock, ...
[-5.0 -5.0] , [5.0 5.0], 5 );
computed = optimsimplex_getall ( s1 );
expected = [
24.19999999999999573674   -1.1999999999999999555911    1.
3347.7382596240795464837  -2.8867513453587889671326    2.5604385416954755783081
71189.511402687028748915  -4.9977886537089943885803  -1.69672908261418342590
211.01779965627284241236    1.6538110421970486640930    1.2839178834110498428345
10770.01508687966997968     3.497452358715236186981     1.857310198247432708740
];
assert_checkalmostequal ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test optimsimplex_randbounds
//
function [ y , myobj ] = mycostf ( x , myobj )
    y = rosenbrock(x);
    myobj.nb = myobj.nb + 1
endfunction

//
// Test randbounds with additional object
//
mydude = tlist(["T_MYSTUFF","nb"]);
mydude.nb = 0;
s1 = optimsimplex_new ();
rand("seed" , 0)
[ s1 , mydude ] = optimsimplex_new ( "randbounds" , [-1.2 1.0], mycostf, ...
[-5.0 -5.0] , [5.0 5.0], 5 , mydude );
computed = optimsimplex_getall ( s1 );
expected = [
24.19999999999999573674   -1.1999999999999999555911    1.
3347.7382596240795464837  -2.8867513453587889671326    2.5604385416954755783081
71189.511402687028748915  -4.9977886537089943885803  -1.69672908261418342590
211.01779965627284241236    1.6538110421970486640930    1.2839178834110498428345
10770.01508687966997968     3.497452358715236186981     1.857310198247432708740
]
assert_checkequal ( mydude.nb , 5 );
s1 = optimsimplex_destroy ( s1 );

// Test unconsistent size of boundsmin
x0 = [1 2];
boundsmin = [-5 -5 -5];
boundsmax = [5 5];
cmd = "newobj = optimsimplex_new ( ""randbounds"" , x0 , rosenbrock , boundsmin , boundsmax );";
instr = "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n";
assert_checkerror ( cmd , instr,[],"optimsimplex_new",4,1,2);

// Test unconsistent size of boundsmax
x0 = [1 2];
boundsmin = [-5 -5];
boundsmax = [5 5 5];
cmd = "newobj = optimsimplex_new ( ""randbounds"" , x0 , rosenbrock , boundsmin , boundsmax );";
instr = "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n";
assert_checkerror ( cmd , instr,[],"optimsimplex_new",5,1,2);

