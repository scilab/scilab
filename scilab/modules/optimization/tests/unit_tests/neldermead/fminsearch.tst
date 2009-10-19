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
function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction
//
// Test basic use without parameters
//
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] );
assert_close ( x , [1.0   1.0], 1e-4 );
assert_close ( fval , 0.0 , 1e-4 );
assert_equal ( exitflag , 1 );
assert_equal ( output.iterations , 85 );
assert_equal ( output.algorithm , "Nelder-Mead simplex direct search" );
assert_equal ( output.funcCount , 159 );
assert_equal ( output.message(1) , "Optimization terminated:");
assert_equal ( output.message(2) , " the current x satisfies the termination criteria using OPTIONS.TolX of 1.000000e-004");
assert_equal ( output.message(3) , " and F(X) satisfies the convergence criteria using OPTIONS.TolFun of 1.000000e-004");
// 
// fminsearch with incorrect number of input arguments
//
cmd = "fminsearch ( )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "fminsearch: Unexpected number of input arguments : 0 provided while 2 or 3 are expected.";
assert_equal ( computed , expected );
//
// Check that tolerance on X is correctly taken into account
//
opt = optimset ( "TolX" , 1.e-2 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e-2 );
assert_close ( fval , 0.0 , 1e-4 );
assert_equal ( exitflag , 1 );
assert_equal ( output.iterations , 70 );
assert_equal ( output.funcCount , 130 );
//
// Check that tolerance on F is correctly taken into account
//
opt = optimset ( "TolFun" , 1.e-10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e-4 );
assert_close ( fval , 0.0 , 1e-2 );
assert_equal ( exitflag , 1 );
assert_equal ( output.iterations , 90 );
assert_equal ( output.funcCount , 168 );
//
// Check that maximum number of iterations is correctly taken into account
//
opt = optimset ( "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e1 );
assert_close ( fval , 0.0 , 1e1 );
assert_equal ( exitflag , 0 );
assert_equal ( output.iterations , 10 );
assert_equal ( output.funcCount , 21 );
//
// Check that maximum number of function evaluations is correctly taken into account
//
opt = optimset ( "MaxFunEvals" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e1 );
assert_close ( fval , 0.0 , 1e1 );
assert_equal ( exitflag , 0 );
assert_equal ( output.iterations , 5 );
assert_equal ( output.funcCount , 11 );
//
// Check that Display is correctly used in mode "final"
//
opt = optimset ( "Display" , "final" );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e-4 );
assert_close ( fval , 0.0 , 1.e-4 );
assert_equal ( exitflag , 1 );
assert_equal ( output.iterations , 85 );
assert_equal ( output.funcCount , 159 );
//
// Check that Display is correctly used in mode "iter"
//
opt = optimset ( "Display" , "iter" );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e-4 );
assert_close ( fval , 0.0 , 1.e-4 );
assert_equal ( exitflag , 1 );
assert_equal ( output.iterations , 85 );
assert_equal ( output.funcCount , 159 );
//
// Check that Display is correctly used in mode "off" (no message at all)
//
opt = optimset ( "Display" , "off" );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e-4 );
assert_close ( fval , 0.0 , 1.e-4 );
assert_equal ( exitflag , 1 );
assert_equal ( output.iterations , 85 );
assert_equal ( output.funcCount , 159 );
//
// Check that Display is correctly used in mode "notify" (display only problem messages)
//
opt = optimset ( "Display" , "notify" );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e-4 );
assert_close ( fval , 0.0 , 1.e-4 );
assert_equal ( exitflag , 1 );
assert_equal ( output.iterations , 85 );
assert_equal ( output.funcCount , 159 );
//
// Check that Display is correctly used in mode "off" (no message at all), when there is a maximum number of iterations reached
//
opt = optimset ( "Display" , "off" , "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e1 );
assert_close ( fval , 0.0 , 1e1 );
assert_equal ( exitflag , 0 );
assert_equal ( output.iterations , 10 );
assert_equal ( output.funcCount , 21 );
//
// Check that Display is correctly used in mode "notify" (display only problem messages), when there is a maximum number of iterations reached
//
opt = optimset ( "Display" , "notify" , "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e1 );
assert_close ( fval , 0.0 , 1e1 );
assert_equal ( exitflag , 0 );
assert_equal ( output.iterations , 10 );
assert_equal ( output.funcCount , 21 );
//
// Check that Display is correctly used in mode "iter", when there is a maximum number of iterations reached
//
opt = optimset ( "Display" , "iter" , "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e1 );
assert_close ( fval , 0.0 , 1e1 );
assert_equal ( exitflag , 0 );
assert_equal ( output.iterations , 10 );
assert_equal ( output.funcCount , 21 );
//
// Check that Display is correctly used in mode "final", when there is a maximum number of iterations reached
//
opt = optimset ( "Display" , "final" , "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_close ( x , [1.0 1.0], 1.e1 );
assert_close ( fval , 0.0 , 1e1 );
assert_equal ( exitflag , 0 );
assert_equal ( output.iterations , 10 );
assert_equal ( output.funcCount , 21 );

//
// Use output function
//
// outfun --
//   A sample output function
// Arguments, input
//   x : the current point
//   optimValues : a tlist which contains the following fields
//     funcCount" : the number of function evaluations
//     fval : the current function value
//     iteration : the current iteration
//     procedure : a string containing the current type of step
//  state : the current state of the algorithm
//    "init", "iter", "done"
//
function outfun ( x , optimValues , state )
  plot( x(1),x(2),'.');
endfunction
opt = optimset ( "OutputFcn" , outfun);
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();
//
// Use several output functions
//
function outfun2 ( x , optimValues , state )
  scf ( fig1 );
  plot( x(1),x(2),'.');
endfunction
function outfun3 ( x , optimValues , state )
  scf ( fig2 );
  plot( x(1),x(2),'o');
endfunction
myfunctions = list ( outfun2 , outfun3 );
fig1 = scf(1000);
fig2 = scf(1001);
opt = optimset ( "OutputFcn" , myfunctions );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(fig1);
close(fig2);
//
// Use plot function
//
//
// plotfun --
//   A sample plot function
// Arguments, input
//   x : the current point
//   optimValues : a tlist which contains the following fields
//     funcCount" : the number of function evaluations
//     fval : the current function value
//     iteration : the current iteration
//     procedure : a string containing the current type of step
//  state : the current state of the algorithm
//    "init", "iter", "done"
//
function plotfun ( x , optimValues , state )
  plot(x(1),x(2),'.');
endfunction
opt = optimset ( "PlotFcns" , plotfun);
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();
//
// Use several plot functions
//
function plotfun2 ( x , optimValues , state )
  scf ( fig1 );
  plot( x(1),x(2),'.');
endfunction
function plotfun3 ( x , optimValues , state )
  scf ( fig2 );
  plot( x(1),x(2),'o');
endfunction
myfunctions = list ( plotfun2 , plotfun3 );
fig1 = scf(1000);
fig2 = scf(1001);
opt = optimset ( "PlotFcns" , myfunctions );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close(fig1);
close(fig2);
//
// Use optimplotfval plot function
//
opt = optimset ( "PlotFcns" , optimplotfval );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();
//
// Use optimplotx plot function
//
opt = optimset ( "PlotFcns" , optimplotx );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();
//
// Use optimplotfunccount plot function
//
opt = optimset ( "PlotFcns" , optimplotfunccount );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );
close();


