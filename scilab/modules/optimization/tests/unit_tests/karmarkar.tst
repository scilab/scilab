// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->




// With slack variables:
//
// Minimize -20.x1 - 24.x2 such as:
// 3.x1 + 6.x2 + x3 = 60
// 4.x1 + 2.x2 + x4 = 32
// x >= 0
c = [-20 -24 0 0]';
Aeq = [
3 6 1 0
4 2 0 1
];
beq = [60 32]';
xexpected = [4 8 0 0]';
fexpected = -272;
x0 = [
4.1128205
7.7333333
1.2615385
0.0820513
];
[xopt,fopt]=karmarkar(Aeq,beq,c,x0);
assert_checkalmostequal ( xopt , xexpected , 1.e-3 , 1.e-3 );
assert_checkalmostequal ( fopt , fexpected , 1.e-4 );
//
// Configure the relative tolerance
rtolf=1.e-6;
[xopt,fopt]=karmarkar(Aeq,beq,c,x0,rtolf);
assert_checkalmostequal ( xopt , xexpected , 1.e-4, 1.e-3  );
assert_checkalmostequal ( fopt , fexpected , 1.e-5 );
//
// Configure the gamma
gam = 0.1;
[xopt,fopt]=karmarkar(Aeq,beq,c,x0,[],gam);
assert_checkalmostequal ( xopt , xexpected , 1.e-3, 1.e-2  );
assert_checkalmostequal ( fopt , fexpected , 1.e-4 );
////////////////////////////////////////////////////////////
//
// Check new API (from Scilab v5.3.x).
//
// Check exit flag
[xopt,fopt,exitflag]=karmarkar(Aeq,beq,c,x0);
assert_checkalmostequal ( xopt , xexpected , 1.e-3, 1.e-3 );
assert_checkalmostequal ( fopt , fexpected , 1.e-4 );
assert_checkequal ( exitflag , 1 );
//
// Check number of iterations
[xopt,fopt,exitflag,iter]=karmarkar(Aeq,beq,c,x0);
assert_checkalmostequal ( xopt , xexpected , 1.e-3, 1.e-3 );
assert_checkalmostequal ( fopt , fexpected , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter>10 , %t );
//
// Check dual solution
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,x0);
lambda.ineqlin = [];
lambda.eqlin = [28/9;8/3];
lambda.upper = [0;0;0;0];
lambda.lower = [0;0;28/9;8/3];
assert_checkalmostequal ( xopt , xexpected , 1.e-3, 1.e-3 );
assert_checkalmostequal ( fopt , fexpected , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter>10 , %t );
assert_checkequal ( yopt.ineqlin , lambda.ineqlin );
assert_checkalmostequal ( yopt.eqlin , lambda.eqlin , 1.e-8 );
assert_checkalmostequal ( yopt.lower , lambda.lower , 1.e-8, 1.e-7 );
assert_checkequal ( yopt.upper , lambda.upper );
//
// Check number of iterations, with default options
[xopt,fopt,exitflag,iter]=karmarkar(Aeq,beq,c,x0,[],[],10);
assert_checkequal ( exitflag , 0 );
assert_checkequal ( iter , 10 );
//
// Check output function
function stop = myoutputfunction ( xopt , optimValues , state )
    localmsg = gettext("Iteration #%3.0f, state=%s, procedure=%s, fopt=%10.3e, x=[%s], dualgap=%10.3e\n")
    xstr = strcat(msprintf("%10.3e\n",xopt)'," ")
    teststring = sprintf(localmsg,optimValues.iteration,state,optimValues.procedure,optimValues.fval,xstr,optimValues.dualgap)
    stop = %f
endfunction
xopt=karmarkar(Aeq,beq,c,x0,[],[],[],myoutputfunction);
assert_checkalmostequal ( xopt , xexpected , 1.e-3 , 1.e-3 );
//
// Check output function, without initial guess
xopt=karmarkar(Aeq,beq,c,[],[],[],[],myoutputfunction);
assert_checkalmostequal ( xopt , xexpected , 1.e-3 , 1.e-3 );
//
// Check that the output function can stop the algorithm
function stop = myoutputfunctionStop ( xopt , optimValues , state )
    stop = (iter >= 7)
endfunction
[xopt,fopt,exitflag,iter]=karmarkar(Aeq,beq,c,x0,[],[],[],myoutputfunctionStop);
assert_checkalmostequal ( xopt , xexpected , 1.e-3 , 1.e-1 );
assert_checkalmostequal ( fopt , fexpected , 1.e-3 );
assert_checkequal ( exitflag , -4 );
assert_checkequal ( iter , 7 );
//
// Check output function with additional arguments
function stop = myoutputfunction2 ( xopt , optimValues , state , myAeq , mybeq , myc )
    localmsg = gettext("Iteration #%3.0f, fopt=%10.3e, state=%s, ||Ax-beq||=%.3e\n")
    teststring = sprintf(localmsg,optimValues.iteration,optimValues.fval,state,norm(myAeq*xopt-mybeq))
    stop = %f
endfunction
xopt=karmarkar(Aeq,beq,c,x0,[],[],[],list(myoutputfunction2,Aeq,beq,c));
assert_checkalmostequal ( xopt , xexpected , 1.e-3 , 1.e-3 );
//
// References
// "Practical Optimization", Antoniou, Lu, 2007
// Chapter 11, "Linear Programming Part I: The simplex method",
// Example 11.9, p. 361
// Chapter 12, "Linear Programming Part II: Interior point methods",
// Example 12.2, p.382
//
// Minimize 2.x1 + 9.x2 + 3.x3
// -2.x1 + 2.x2 + x3 - x4 = 1
//    x1 + 4.x2 - x3 - x5 = 1
// x >= 0
Aeq = [
-2 2 1 -1 0
1 4 -1 0 -1
];
beq = [1;1];
c = [2;9;3;0;0];
x0 = [0.2;0.7;1;1;1];
gam = 0.9999;
rtolf = 1.e-4;
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,x0,rtolf,gam,[],myoutputfunction);
xstar = [0 1/3 1/3 0 0]';
fstar = 4;
lambda.ineqlin = [];
lambda.eqlin = [-3.5;-0.5];
lambda.upper = [0;0;0;0;0];
lambda.lower = [8.5;0;0;3.5;0.5];
assert_checkalmostequal ( xopt , xstar , 1.e-4 , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter , 4 );
assert_checkequal ( yopt.ineqlin , lambda.ineqlin );
assert_checkalmostequal ( yopt.eqlin , lambda.eqlin , 1.e-6 , 1.e-6 );
assert_checkalmostequal ( yopt.lower , lambda.lower , 1.e-6 , 1.e-6 );
assert_checkequal ( yopt.upper , lambda.upper );
//
// Minimize -x1 -x2
// x1 - x2      = 0
// x1 + x2 + x3 = 2
// x >= 0
//
// Let karmarkar find a feasible x0.
Aeq = [
1 -1 0
1  1 1
];
beq = [0;2];
c = [-1;-1;0];
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c);
xstar = [1;1;0];
fstar = -2;
lambda.ineqlin = [];
lambda.eqlin = [0;1];
lambda.upper = [0;0;0];
lambda.lower = [0;0;1];
assert_checkalmostequal ( xopt , xstar , 1.e-4 , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkequal ( yopt.ineqlin , lambda.ineqlin );
assert_checkalmostequal ( yopt.eqlin , lambda.eqlin , 1.e-6 , 1.e-15);
assert_checkalmostequal ( yopt.lower , lambda.lower , 1.e-6 , 1.e-6 );
assert_checkequal ( yopt.upper , lambda.upper );
//
// Give a linear inequality A*x <= b.
//
// Minimize -x1 -x2
// x1 - x2  = 0
// x1 + x2 <= 2
//
// Give x0.
Aeq = [
1 -1
];
beq = 0;
c = [-1;-1];
A = [1 1];
b = 2;
x0 = [0.1;0.1];
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,x0,[],[],[],[],A,b);
xstar=[1 1]';
fstar = c'*xstar;
lambda.ineqlin = 1;
lambda.eqlin = 0;
lambda.upper = [0;0];
lambda.lower = [0;0];
assert_checkalmostequal ( xopt , xstar , 1.e-4 , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkalmostequal ( yopt.ineqlin , lambda.ineqlin , 1.e-6 );
assert_checkalmostequal ( yopt.eqlin , lambda.eqlin , [] , 1.e-6 );
assert_checkequal ( yopt.lower , lambda.lower );
assert_checkequal ( yopt.upper , lambda.upper );
//
// Give a linear inequality A*x <= b
//
// Minimize -x1 -x2
// x1 - x2  = 0
// x1 + x2 <= 2
//
// Do not give x0.
Aeq = [
1 -1
];
beq = 0;
c = [-1;-1];
A = [1 1];
b = 2;
function stop = myoutputfunction3 ( xopt , optimValues , state )
    assert_checkequal ( size(xopt) , [2 1] );
    assert_checkequal ( or(state==["init","iter","done"]) , %t );
    assert_checkequal ( or(optimValues.procedure==["x0","x*"]) , %t );
    stop = %f
endfunction
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,[],[],[],[],myoutputfunction3,A,b);
xstar=[1 1]';
fstar = c'*xstar;
lambda.ineqlin = 1;
lambda.eqlin = 0;
lambda.upper = [0;0];
lambda.lower = [0;0];
assert_checkalmostequal ( xopt , xstar , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkalmostequal ( yopt.ineqlin , lambda.ineqlin , 1.e-6 );
assert_checkalmostequal ( yopt.eqlin , lambda.eqlin , [] , 1.e-6 );
assert_checkequal ( yopt.lower , lambda.lower );
assert_checkequal ( yopt.upper , lambda.upper );
//
// Minimize -20.x1 - 24.x2 such as:
// 3.x1 + 6.x2 >= 60
// 4.x1 + 2.x2 >= 32
c = [-20 -24]';
Aeq=[];
beq=[];
A = [
3 6
4 2
];
b = [60 32]';
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,[],[],[],[],[],A,b);
xstar = [4 8]';
fstar = c'*xstar;
lambda.ineqlin = [28/9;8/3];
lambda.eqlin = [];
lambda.upper = [0;0];
lambda.lower = [0;0];
assert_checkalmostequal ( xopt , xstar , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkalmostequal ( yopt.ineqlin , lambda.ineqlin , 1.e-6 );
assert_checkequal ( yopt.eqlin , lambda.eqlin );
assert_checkequal ( yopt.lower , lambda.lower );
assert_checkequal ( yopt.upper , lambda.upper );
//
// References
// "Practical Optimization", Antoniou, Lu, 2007
// Chapter 11, "Linear Programming Part I: The simplex method",
// Example 11.9, p. 361
// Chapter 12, "Linear Programming Part II: Interior point methods",
// Example 12.2, p.382
//
// Minimize 2.x1 + 9.x2 + 3.x3
// +2.x1 - 2.x2 - x3 <= -1
//   -x1 - 4.x2 + x3 <= -1
// x >= 0
//
// Give x0
Aeq=[];
beq=[];
A = [
2 -2 -1
-1 -4  1
-1  0  0
0 -1  0
0  0 -1
];
b = [-1;-1;0;0;0];
c = [2;9;3];
x0 = [0.2;0.7;1];
[xopt,fopt,exitflag,iter]=karmarkar(Aeq,beq,c,x0,[],[],[],[],A,b);
xstar = [0 1/3 1/3]';
fstar = c'*xstar;
assert_checkalmostequal ( xopt , xstar , 1.e-4 , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
//
// Let x0 be found by the algorithm.
[xopt,fopt,exitflag,iter]=karmarkar(Aeq,beq,c,[],[],[],[],[],A,b);
xstar = [0 1/3 1/3]';
fstar = c'*xstar;
assert_checkalmostequal ( xopt , xstar , 1.e-4 , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
/////////////////////////////////////////////////////
// References
// "Lipsol toolbox", Yin Zhang, Scilab port by Rubio Scola, example0.sce.
//
// Minimize 2.x1 + 5.x2 - 2.5.x3
//    x1        S4 x3 <= 5
// E2 x1 - x2    - x3 <= 0
//    x1              <= 2
//         x2         <= %inf
//                 x3 <= 3
//  - x1              <= 2
//       - x2         <= -1
//               - x3 <= 0
//
// where
// S4 = sin(pi/4)/4
// E2 = exp(2)
//

S4 = sin(%pi/4)/4;
E2 = exp(2);
c = [ 2; 5; -2.5];
A = [
1  0 S4
E2 -1 -1
1  0  0
0  0  1
-1  0  0
0 -1  0
0  0 -1
];
b = [ 5; 0;2;3;2;-1;0];
Aeq = [];
beq = [];
xstar = [-2;1;3];
fstar = c'*xstar;
lambda.ineqlin = [0;0;0;2.5;2;5;0];
lambda.eqlin = [];
lambda.upper = [0;0;0];
lambda.lower = [0;0;0];
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,[],[],[],[],[],A,b);
assert_checkalmostequal ( xopt , xstar , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkalmostequal ( yopt.ineqlin , lambda.ineqlin , 1.e-9 , 1.e-10 );
assert_checkequal ( yopt.eqlin , lambda.eqlin );
assert_checkequal ( yopt.lower , lambda.lower );
assert_checkequal ( yopt.upper , lambda.upper );

//
// Minimize 2.x1 + 9.x2 + 3.x3
//  2 x1 - 2 x2 - x3 <= -1
// -1 x1 - 4 x2 + x3 <= -1
// - x1              <= -1
// - x2              <= 0
// - x3              <= 0
//
Aeq=[];
beq=[];
A = [
2 -2 -1
-1 -4  1
-1  0  0
0 -1  0
0  0 -1
];
b = [-1;-1;-1;0;0];
c = [2;9;3];
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,[],[],[],[],[],A,b);
xstar = [1;0.5;2];
fstar = c'*xstar;
lambda.ineqlin = [3.5;0.5;8.5;0;0];
lambda.eqlin = [];
lambda.upper = [0;0;0];
lambda.lower = [0;0;0];
assert_checkalmostequal ( xopt , xstar , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkalmostequal ( yopt.ineqlin , lambda.ineqlin , 1.e-9 , 1.e-8 );
assert_checkequal ( yopt.eqlin , lambda.eqlin );
assert_checkequal ( yopt.lower , lambda.lower );
assert_checkequal ( yopt.upper , lambda.upper );

//////////////////////////////////////////////////////////////////////////
//
// Set lower bound and do not give x0.
Aeq=[];
beq=[];
A = [
2 -2 -1
-1 -4  1
];
b = [-1;-1];
c = [2;9;3];
lb = [1;0;0];
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,[],[],[],[],[],A,b,lb);
xstar = [1;0.5;2];
fstar = c'*xstar;
lambda.ineqlin = [3.5;0.5];
lambda.eqlin = [];
lambda.upper = [0;0;0];
lambda.lower = [8.5;0;0];
assert_checkalmostequal ( xopt , xstar , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkalmostequal ( yopt.ineqlin , lambda.ineqlin , 1.e-8 );
assert_checkequal ( yopt.eqlin , lambda.eqlin );
assert_checkalmostequal ( yopt.lower , lambda.lower , 1.e-9 , 1.e-8 );
assert_checkequal ( yopt.upper , lambda.upper );
//
// Set lower bound and give x0.
Aeq=[];
beq=[];
A = [
2 -2 -1
-1 -4  1
];
b = [-1;-1];
c = [2;9;3];
x0 = [1.337848;0.885225;2.535279];
lb = [1;0;0];
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,x0,[],[],[],[],A,b,lb);
xstar = [1;0.5;2];
fstar = c'*xstar;
lambda.ineqlin = [3.5;0.5];
lambda.eqlin = [];
lambda.upper = [0;0;0];
lambda.lower = [8.5;0;0];
assert_checkalmostequal ( xopt , xstar , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkalmostequal ( yopt.ineqlin , lambda.ineqlin , 1.e-9 , 1.e-8 );
assert_checkalmostequal ( yopt.eqlin , lambda.eqlin , %eps , 1.e-8 );
assert_checkalmostequal ( yopt.lower , lambda.lower , 1.e-9 , 1.e-8 );
assert_checkalmostequal ( yopt.upper , lambda.upper , 1.e-9 );
// References
// LIPSOL is a set of Linear-programming Interior-Point SOLvers written
// by Yin Zhang.
// The original Matlab-based code has been adapted to Scilab
// by H. Rubio Scola.
//
// Minimize 2 x1 + 5 x2 - 2.5 x3
//    x1 +   S4 x3 <= 5
// E2 x1 - x2 - x3 <= 0
//  -2 <= x1 <= 2
//   1 <= x2
//   0 <= x3 <= 3
// where:
// S4 = sin(pi/4)/4
// E2 = exp(2)
//
// Do not give x0.
c = [ 2; 5; -2.5];
S4 = sin(%pi/4)/4;
E2 = exp(2);
A = [
1  0 S4
E2 -1 -1
];
b = [ 5; 0];
lb = [ -2; 1   ; 0 ];
ub = [  2; %inf; 3 ];
Aeq = [];
beq = [];
[xopt,fopt,exitflag,iter,yopt]=karmarkar(Aeq,beq,c,[],[],[],[],[],A,b,lb,ub);
xstar = [-2;1;3];
fstar = c'*xstar;
lambda.ineqlin = [0;0];
lambda.eqlin = [];
lambda.upper = [0;0;2.5];
lambda.lower = [2;5;0];
assert_checkalmostequal ( xopt , xstar , 1.e-4 );
assert_checkalmostequal ( fopt , fstar , 1.e-4 );
assert_checkequal ( exitflag , 1 );
assert_checkequal ( iter > 0 , %t );
assert_checkalmostequal ( yopt.ineqlin , lambda.ineqlin , [] , 1.e-9 );
assert_checkequal ( yopt.eqlin , lambda.eqlin );
assert_checkalmostequal ( yopt.lower , lambda.lower , 1.e-9 , 1.e-10 );
assert_checkalmostequal ( yopt.upper , lambda.upper , 1.e-9 , 1.e-10 );
//
// An unbounded problem.
c = [-20 -24]';
A = [
-3 -6
-4 -2
];
b = [-60 -32]';
[xopt,fopt,exitflag,iter,yopt]=karmarkar([],[],c,[],[],[],[],[],A,b);
assert_checkequal ( exitflag , -2 );
//
// "Linear Programming in Matlab"
// Ferris, Mangasarian, Wright
// 2008
// Chapter 3, "The Simplex Method", Exercise 3-4-2 1.
//
// An infeasible problem.
// Minimize -3 x1 + x2
//  - x1 -   x2 >= -2
//  2 x1 + 2 x2 >= 10
// x >= 0
c = [-3;1];
A=[
-1 -1
2  2
];
A=-A;
b=[-2;10];
b=-b;
lb=[0;0];
[xopt,fopt,exitflag,iter,yopt]=karmarkar([],[],c,[],[],[],[],[],A,b,lb);
assert_checkequal ( xopt , [] );
assert_checkequal ( fopt , [] );
assert_checkequal ( exitflag , -1 );
assert_checkequal ( iter > 0 , %t );
assert_checkequal ( yopt.ineqlin , [] );
assert_checkequal ( yopt.eqlin , [] );
assert_checkequal ( yopt.lower , [] );
assert_checkequal ( yopt.upper , [] );


//
// "Linear Programming in Matlab"
// Ferris, Mangasarian, Wright
// 2008
// Chapter 3, "The Simplex Method", Exercise 3-4-2 2.
//
// An unbounded problem.
// Minimize -x1 + x2
//  2 x1 -   x2 >= 1
//    x1 + 2 x2 >= 2
// x >= 0
c = [-1;1];
A=[
2 -1
1  2
];
A=-A;
b=[1;2];
b=-b;
lb=[0;0];

[xopt,fopt,exitflag,iter,yopt]=karmarkar([],[],c,[],[],[],[],[],A,b,lb);
assert_checkequal ( exitflag , -2 );

//
// "Linear and Nonlinear Optimization"
// Griva, Nash, Sofer
// 2009
// Chapter 5, "The Simplex Method", Example 5.3
//
// An unbounded problem.
// Minimize -x1 - 2 x2
//  -1 x1 +   x2 <= 2
//  -2 x1 +   x2 <= 1
// x >= 0
c = [-1;-2];
A=[
-1  1
-2  1
];
b=[2;1];
lb=[0;0];
[xopt,fopt,exitflag,iter,yopt]=karmarkar([],[],c,[],[],[],[],[],A,b,lb);
assert_checkequal ( exitflag , -2 );
//
// "Linear and Nonlinear Optimization"
// Griva, Nash, Sofer
// 2009
// Chapter 5, "The Simplex Method", Example 5.6
//
// An unfeasible problem.
// Minimize -x1
//  -  x1 -   x2 <= -6
//   2 x1 + 3 x2 <=  4
// x >= 0
c = [-1;0];
A=[
-1  -1
2   3
];
b=[-6;4];
lb=[0;0];
[xopt,fopt,exitflag,iter,yopt]=karmarkar([],[],c,[],[],[],[],[],A,b,lb);
assert_checkequal ( exitflag , -1 );
//
// Example from the help page
// Check that the output points remain feasible
function stop = myoutputfunction4 ( xopt , optimValues , state , A , b )
    assert_checkequal ( and(A*xopt<=b) , %t );
    stop = %f
endfunction
n=11;
A = [2*linspace(0,1,n)',ones(n,1)];
b = 1 + linspace(0,1,n)'.^2;
c=[-1;-1];
xopt=karmarkar([],[],c,[],[],[],[],list(myoutputfunction4,A,b),A,b);
xstar = [0.5005127;0.7494803];
assert_checkalmostequal ( xopt , xstar , 1.e-4 );


