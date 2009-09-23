// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
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
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_setallfv: The number of rows in the function value array is 4, while expected 5.";
assert_equal ( computed , expected );
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
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_setallx: The number of rows is 4, while expected 5.";
assert_equal ( computed , expected );
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
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_setallx: The number of columns is 1, while expected 2.";
assert_equal ( computed , expected );
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
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_check: Number of rows of x is 5 is different from number of vertices = 6.";
assert_equal ( computed , expected );
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
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_check: Number of rows of fv is 6 is different from number of vertices = 5.";
assert_equal ( computed , expected );
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
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_setallfv: The number of columns in the function value array is 2, while expected 1.";
assert_equal ( computed , expected );
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
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_check: Number of columns of x is 2 is different from dimension = 3.";
assert_equal ( computed , expected );
s1 = optimsimplex_destroy(s1);

