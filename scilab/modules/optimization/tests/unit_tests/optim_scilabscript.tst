// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
//
// Do not check ref, because imp option create output messages
// which contains lots of floating point values which may
// be slightly different across platforms, without being bugs.
//
// optim_script.tst --
//   Test the optim command with the Rosenbrock test case 
//   in the case where the cost function is a Scilab function.
//   The expected solution is x=(1,...,1) where f(x)=0
//


Leps=1.e-3;
bs=[5 5];
bi=-bs;
x0=[-1.2 1];
xopt=[1 1];
fopt = 0.0;
gopt = [0.0 0.0];
function [ f , g , ind ] = rosenbrock ( x , ind )
  if ind == 2 | ind == 4 then
    f = 100.0 *(x(2)-x(1)^2)^2 + (1-x(1))^2;
  end
  if ind == 2 | ind == 4 then
    g(1) = - 400. * ( x(2) - x(1)**2 ) * x(1) -2. * ( 1. - x(1) )
    g(2) = 200. * ( x(2) - x(1)**2 )
  end
endfunction
//
// Configure the test so that verbose message do not interrupt the test.
lines(0);
//
// 1. Test unconstrained BFGS
// Test without arguments
[f,x,g,tr]=optim(rosenbrock,x0);
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with maximum number of call to cost function
[f,x,g]=optim(rosenbrock,x0,'qn','ar',50);
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with hot-restart array tr
[f,x,g,tr]=optim(rosenbrock,x0,'qn','ar',50);
[f,x,g]=optim(rosenbrock,x0,tr);
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with maximum number of call to cost function, maximum number of iterations
[f,x,g]=optim(rosenbrock,x0,'qn','ar', 50 , 100 );
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with various imp levels
// imp = 0 : No messages
[f,x,g]=optim(rosenbrock,x0,'qn',imp=0);
// imp = 1 : one line at start, one line at end
[f,x,g]=optim(rosenbrock,x0,'qn',imp=1);
// imp = 2 : one line by iteration
[f,x,g]=optim(rosenbrock,x0,'qn',imp=2);
// imp = 3 : one line by iteration + one line by line search
[f,x,g]=optim(rosenbrock,x0,'qn',imp=3);
// Negative imp : call back the cost function with ind = 0 each -imp iteration
[f,x,g]=optim(rosenbrock,x0,'qn',imp=-5);
//
// 2. Test unconstrained Limited Memory BFGS
[f,x,g]=optim(rosenbrock,x0,'gc');
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
[f,x,g]=optim(rosenbrock,x0,'gc','ar',50);
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with various imp levels
// imp = 0 : No messages
[f,x,g]=optim(rosenbrock,x0,'gc',imp=0);
// imp = 1,2 : one line at start, one line at end
[f,x,g]=optim(rosenbrock,x0,'gc',imp=1);
// imp = 3 : one line by iteration
[f,x,g]=optim(rosenbrock,x0,'gc',imp=2);
// imp = 4 : one line by iteration + one line by line search
[f,x,g]=optim(rosenbrock,x0,'gc',imp=3);
//
// 3. Test unconstrained non-differentiable method
[f,x,g]=optim(rosenbrock,x0,'nd');
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with maximum number of call to cost function
[f,x,g]=optim(rosenbrock,x0,'nd','ar',100);
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with maximum number of call to cost function, maximum number of iterations
[f,x,g]=optim(rosenbrock,x0,'nd','ar', 100 , 100 );
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with various imp levels
// imp = 0 : No messages
[f,x,g]=optim(rosenbrock,x0,'nd',imp=0);
// imp = 1,2 : one line at start, one line at end
[f,x,g]=optim(rosenbrock,x0,'nd',imp=1);
// imp = 3 : one line by iteration
[f,x,g]=optim(rosenbrock,x0,'nd',imp=2);
// imp = 4 : one line by iteration + one line by line search
[f,x,g]=optim(rosenbrock,x0,'nd',imp=3);

//
// 4. Test bound-constrained BFGS
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'qn');
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with maximum number of call to cost function
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'qn','ar',50);
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with maximum number of call to cost function, maximum number of iterations
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'qn','ar', 100 , 100 );
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with various imp levels
// imp = 0 : No messages
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'qn',imp=0);
// imp = 1,2 : one line at start, one line at end
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'qn',imp=1);
// imp = 3 : one line by iteration
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'qn',imp=2);
// imp = 4 : one line by iteration + one line by line search
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'qn',imp=3);
//
// 5. Test bound-constrained Limited Memory BFGS
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'gc');
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with maximum number of call to cost function
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'gc','ar',100);
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with maximum number of call to cost function, maximum number of iterations
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'gc','ar', 100 , 100 );
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
// Test with various imp levels
// imp = 0 : No messages
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'gc',imp=0);
// imp = 1,2 : one line at start, one line at end
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'gc',imp=1);
// imp = 3 : one line by iteration
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'gc',imp=2);
// imp = 4 : one line by iteration + one line by line search
[f,x,g]=optim(rosenbrock,'b',bi,bs,x0,'gc',imp=3);
//
// 6. Test "sd", "si", "td", "ti" options
[f,x,g,td]=optim(rosenbrock,x0,'sd');
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
[f,x,g,ti]=optim(rosenbrock,x0,'si');
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
[f,x,g,ti,td]=optim(rosenbrock,x0,'si','sd');
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );
[f,x,g,ti,td]=optim(rosenbrock,x0,"ti",[2 2 2],"td",[1.1 1.1 1.1],'si','sd');
assert_checkalmostequal ( x , xopt , Leps );
assert_checkalmostequal ( f , fopt , [] , Leps );
assert_checkalmostequal ( g , gopt , [] , Leps );



