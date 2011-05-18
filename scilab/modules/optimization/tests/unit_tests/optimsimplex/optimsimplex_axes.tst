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

function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test with x0 only
//
s1 = optimsimplex_new ( "axes" , [-1.2 1.0] );
computed = optimsimplex_getall ( s1 );
expected = [
    0.  -1.2    1.  
    0.  -0.2    1.  
    0.  -1.2    2.  
];
assert_checkalmostequal ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with function only
//
s1 = optimsimplex_new ( "axes" , [-1.2 1.0] , rosenbrock );
computed = optimsimplex_getall ( s1 );
expected = [
    24.2  -1.2    1.  
    93.6  -0.2    1.  
    36.2  -1.2    2.  
];
assert_checkalmostequal ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with a scalar length
//
s1 = optimsimplex_new ( "axes" , [-1.2 1.0] , rosenbrock , 2.0 );
computed = optimsimplex_getall ( s1 );
expected = [
    24.2   -1.2    1.  
    13.      0.8    1.  
    248.2  -1.2    3.  
];
assert_checkalmostequal ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with a vector length
//
s1 = optimsimplex_new ( "axes" , [-1.2 1.0] , rosenbrock , [1.0 2.0] );
computed = optimsimplex_getall ( s1 );
expected = [
24.2 -1.2 1.
93.6 -0.2 1.
248.2 -1.2 3.
];
assert_checkalmostequal ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with a scalar length and an additionnal object
//
myobj = tlist(["T_MYSTUFF","nb"]);
myobj.nb = 0;
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction
[ s1 , myobj ] = optimsimplex_new ( "axes" , [-1.2 1.0] , mycostf , 1.0, myobj );
computed = optimsimplex_getall ( s1 );
expected = [
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
assert_checkalmostequal ( computed , expected , %eps );
assert_checkequal ( myobj.nb , 3 );
nbve = optimsimplex_getnbve ( s1 );
assert_checkequal ( nbve , 3 );
s1 = optimsimplex_destroy ( s1 );

// Test with a unconsistent x0 and len
cmd = "newobj = optimsimplex_new ( ""axes"" , [1 2] , rosenbrock , [1 2 3] )";
assert_checkerror(cmd, "%s: The len vector is not consistent with the x0 point. Current shape of x0 is %d x %d while current shape of len is %d x %d.", [],"optimsimplex_axes", 1, 2, 1, 3);

// Test with a unconsistent x0
cmd = "newobj = optimsimplex_new ( ""axes"" , [1 2;3 4] , rosenbrock )";
assert_checkerror(cmd,"%s: The x0 vector is expected to be a row matrix, but current shape is %d x %d.",[],"optimsimplex_axes",2,2);

// Test with a unconsistent len
cmd = "newobj = optimsimplex_new ( ""axes"" , [1 2] , rosenbrock , ""foo"" )";
assert_checkerror(cmd,"%s: Expected real variable for variable %s at input #%d, but got %s instead.",[],"assert_typereal", "len" , 1 , "string" );
