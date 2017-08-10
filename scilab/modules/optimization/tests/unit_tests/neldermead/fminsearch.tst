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


function y = rosenbrock ( x )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction
//
// Test basic use without parameters
//
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] );
assert_checkalmostequal ( x , [1.0   1.0], 1e-4 );
assert_checkalmostequal ( fval , 0.0 , [] , 1e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( output.iterations , 85 );
assert_checkequal ( output.algorithm , "Nelder-Mead simplex direct search" );
assert_checkequal ( output.funcCount , 159 );
assert_checkequal ( output.message(1) , "Optimization terminated:");
assert_checkequal ( output.message(2) , " the current x satisfies the termination criteria using OPTIONS.TolX of 0.0001");
assert_checkequal ( output.message(3) , " and F(X) satisfies the convergence criteria using OPTIONS.TolFun of 0.0001");
// 
// fminsearch with incorrect number of input arguments
//
cmd = "fminsearch ( )";
assert_checkerror(cmd,"%s: Wrong number of input arguments: %d or %d expected.\n",[], "fminsearch",2,3);
//
// Check that tolerance on X is correctly taken into account
//
opt = optimset ( "TolX" , 1.e-2 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e-2 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( output.iterations , 70 );
assert_checkequal ( output.funcCount , 130 );
//
// Check that tolerance on F is correctly taken into account
//
opt = optimset ( "TolFun" , 1.e-10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e-4 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1e-2 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( output.iterations , 90 );
assert_checkequal ( output.funcCount , 168 );
//
// Check that maximum number of iterations is correctly taken into account
//
opt = optimset ( "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e1 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1e1 );
assert_checkequal ( exitflag , 0 );
assert_checkequal ( output.iterations , 10 );
assert_checkequal ( output.funcCount , 21 );
//
// Check that maximum number of function evaluations is correctly taken into account
//
opt = optimset ( "MaxFunEvals" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e1 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1e1 );
assert_checkequal ( exitflag , 0 );
assert_checkequal ( output.iterations , 5 );
assert_checkequal ( output.funcCount , 11 );
//
// Check that Display is correctly used in mode "final"
//
opt = optimset ( "Display" , "final" );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e-4 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( output.iterations , 85 );
assert_checkequal ( output.funcCount , 159 );
//
// Check that Display is correctly used in mode "iter"
//
opt = optimset ( "Display" , "iter" );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e-4 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( output.iterations , 85 );
assert_checkequal ( output.funcCount , 159 );
//
// Check that Display is correctly used in mode "off" (no message at all)
//
opt = optimset ( "Display" , "off" );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e-4 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( output.iterations , 85 );
assert_checkequal ( output.funcCount , 159 );
//
// Check that Display is correctly used in mode "notify" (display only problem messages)
//
opt = optimset ( "Display" , "notify" );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e-4 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( output.iterations , 85 );
assert_checkequal ( output.funcCount , 159 );
//
// Check that Display is correctly used in mode "off" (no message at all), when there is a maximum number of iterations reached
//
opt = optimset ( "Display" , "off" , "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e1 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1e1 );
assert_checkequal ( exitflag , 0 );
assert_checkequal ( output.iterations , 10 );
assert_checkequal ( output.funcCount , 21 );
//
// Check that Display is correctly used in mode "notify" (display only problem messages), when there is a maximum number of iterations reached
//
opt = optimset ( "Display" , "notify" , "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e1 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1e1 );
assert_checkequal ( exitflag , 0 );
assert_checkequal ( output.iterations , 10 );
assert_checkequal ( output.funcCount , 21 );
//
// Check that Display is correctly used in mode "iter", when there is a maximum number of iterations reached
//
opt = optimset ( "Display" , "iter" , "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e1 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1e1 );
assert_checkequal ( exitflag , 0 );
assert_checkequal ( output.iterations , 10 );
assert_checkequal ( output.funcCount , 21 );
//
// Check that Display is correctly used in mode "final", when there is a maximum number of iterations reached
//
opt = optimset ( "Display" , "final" , "MaxIter" , 10 );
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1] , opt );
assert_checkalmostequal ( x , [1.0 1.0], 1.e1 );
assert_checkalmostequal ( fval , 0.0 , [] ,  1e1 );
assert_checkequal ( exitflag , 0 );
assert_checkequal ( output.iterations , 10 );
assert_checkequal ( output.funcCount , 21 );

//
// Test basic use with column x0
//
[x , fval , exitflag , output] = fminsearch ( rosenbrock , [-1.2 1].' );
assert_checkalmostequal ( x , [1.0   1.0], 1e-4 );

//
// Test extra arguments
function y = bananaext (x,a,b)
  y = a*(x(2)-x(1)^2)^2 + (b-x(1))^2;
endfunction
a = 100;
b = 1;
[x fval] = fminsearch ( list(bananaext,a,b) , [-1.2 1] );
assert_checkalmostequal ( x , [1.0   1.0], 1e-4 );
assert_checkalmostequal ( fval , 0, [], 1e-5 );
//
a = 100;
b = 12;
[x fval] = fminsearch ( list(bananaext,a,b) , [10 100] );
assert_checkalmostequal ( x , [12  144], 1e-4 );
assert_checkalmostequal ( fval , 0, [], 1e-5 );
