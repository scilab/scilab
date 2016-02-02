// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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

funcprot(0);

function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test optimsimplex_check.
//
//
// Test with 3 vertices
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,3);
newsimplex = [
    1.    2.  
    3.    4.  
    5.    6.  
];
s1 = optimsimplex_setallx ( s1 , newsimplex );
s1 = optimsimplex_setallfv ( s1 , [3.0 4.0 5.0]' );
optimsimplex_check ( s1 );
s1 = optimsimplex_destroy(s1);
//
// Test with empty simplex
//
s1 = optimsimplex_new ();
optimsimplex_check ( s1 );
s1 = optimsimplex_destroy(s1);
//
// Test with 3 vertices
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,5);
newsimplex = [
    1.    2.   
    3.    4.   
    5.    6.   
    7.    8.   
    9.    10.  
];
s1 = optimsimplex_setallx ( s1 , newsimplex );
s1 = optimsimplex_setallfv ( s1 , [3.0 4.0 5.0 6.0 7.0]' );
optimsimplex_check ( s1 );
s1 = optimsimplex_destroy(s1);

//
// Test error case : number of function values does not match number of vertices
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setnbve(s1,5);
cmd = "s1 = optimsimplex_setallfv ( s1 , [3.0 4.0 5.0 6.0]'' );";
assert_checkerror ( cmd , "%s: The number of rows in the function value array is %d, while expected %d.",[], "optimsimplex_setallfv" , 4 , 5);
s1 = optimsimplex_destroy(s1);
//
// Test error case : number of columns does not match number of vertices
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,5);
newsimplex = [
    1.    2.  
    3.    4.  
    5.    6.  
    7.    8.  
];
cmd = "s1 = optimsimplex_setallx ( s1 , newsimplex );";
assert_checkerror(cmd,"%s: The number of rows in x is %d, while expected %d.", [],"optimsimplex_setallx" , 4 , 5 );
s1 = optimsimplex_destroy(s1);
//
// Test error case : number of columns does not match dimension
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,5);
newsimplex = [
1.0 
3.0 
5.0 
7.0 
8.0
];
cmd = "s1 = optimsimplex_setallx ( s1 , newsimplex );";
assert_checkerror(cmd,"%s: The number of columns in x is %d, while expected %d.", [], "optimsimplex_setallx" , 1 , 2);
s1 = optimsimplex_destroy(s1);
//
// Test error case : dimension in the x array does not match the value of n
// because of unconsistent configurations
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,5);
newsimplex = [
    1.    1.  
    3.    3.  
    5.    5.  
    7.    7.  
    8.    8.  
];
s1 = optimsimplex_setallx ( s1 , newsimplex );
s1 = optimsimplex_setnbve(s1,6);
s1 = optimsimplex_setallfv ( s1 , [3.0 4.0 5.0 6.0 7.0 8.0]' );
cmd = "optimsimplex_check ( s1 );";
assert_checkerror(cmd,"%s: Number of rows of x is %d, which is different from number of vertices = %d.",[],"optimsimplex_check" , 5 , 6);
s1 = optimsimplex_destroy(s1);
//
// Test error case : number of vertices in the fv array does not match nbve 
// because of unconsistent configurations
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setnbve(s1,6);
s1 = optimsimplex_setallfv ( s1 , [3.0 4.0 5.0 6.0 7.0 8.0]' );
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,5);
newsimplex = [
    1.    1.  
    3.    3.  
    5.    5.  
    7.    7.  
    8.    8.  
];
s1 = optimsimplex_setallx ( s1 , newsimplex );
cmd = "optimsimplex_check ( s1 );";
assert_checkerror(cmd,"%s: Number of rows of fv is %d, which is different from number of vertices = %d.",[],"optimsimplex_check" , 6 , 5);
s1 = optimsimplex_destroy(s1);
//
// Test error case : unconsistent number of columns in the fv array
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setnbve(s1,6);
fv = [
3.0 3.0
4.0 4.0
5.0 5.0
6.0 6.0
7.0 7.0
8.0 8.0
];
cmd = "s1 = optimsimplex_setallfv ( s1 , fv );";
assert_checkerror(cmd,"%s: The number of columns in the function value array is %d, while expected 1.", [],"optimsimplex_setallfv" , 2);
s1 = optimsimplex_destroy(s1);
//
// Test error case : number of vertices in the x array does not match nbve 
// because of unconsistent configurations
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,5);
s1 = optimsimplex_setallfv ( s1 , [3.0 4.0 5.0 6.0 7.0]' );
newsimplex = [
    1.    1.  
    3.    3.  
    5.    5.  
    7.    7.  
    8.    8.  
];
s1 = optimsimplex_setallx ( s1 , newsimplex );
s1 = optimsimplex_setn(s1,3);
cmd = "optimsimplex_check ( s1 );";
assert_checkerror(cmd,"%s: Number of columns of x is %d, which is different from dimension = %d.",[],"optimsimplex_check" , 2 , 3);
s1 = optimsimplex_destroy(s1);

funcprot(1);
