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

//
// Spendley basic case
//
s1 = optimsimplex_new ( "spendley" , [-1.2 1.0] );
computed = optimsimplex_getall ( s1 );
expected = [
    0.0   -1.2    1.                        
    0.0   -0.2340741737109317543997    1.2588190451025207394764  
    0.0  -0.9411809548974792161147    1.9659258262890682011914  
];
assert_checkalmostequal ( computed , expected , 1.e-6 );
s1 = optimsimplex_destroy ( s1 );

//
// Spendley basic case
//
s1 = optimsimplex_new ( "spendley" , [-1.2 1.0] , rosenbrock );
computed = optimsimplex_getall ( s1 );
expected = [
    24.2   -1.2    1.                        
    146.4913601204771680386   -0.2340741737109317543997    1.2588190451025207394764  
    120.43069965448485447723  -0.9411809548974792161147    1.9659258262890682011914  
];
assert_checkalmostequal ( computed , expected , 1.e-6 );
s1 = optimsimplex_destroy ( s1 );

//
// Spendley, case with object
//

myobj = tlist(["T_MYSTUFF","nb"]);
myobj.nb = 0;
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction
s1 = optimsimplex_new ();
[ s1 , myobj ] = optimsimplex_new ( "spendley" , [-1.2 1.0] , mycostf , 1.0 , myobj );
computed = optimsimplex_getall ( s1 );
expected = [
    24.2   -1.2    1.                        
    146.4913601204771680386   -0.2340741737109317543997    1.2588190451025207394764  
    120.43069965448485447723  -0.9411809548974792161147    1.9659258262890682011914  
];
assert_checkalmostequal ( computed , expected , 10 * %eps );
assert_checkequal ( myobj.nb , 3 );
s1 = optimsimplex_destroy ( s1 );


