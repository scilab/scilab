// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 Scilab Enterprises  Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// unit tests for qmr() function 
// =============================================================================
warning('off')

// ----------------
// If A is a matrix
// ----------------

// 1. Interface
// ============
assert_checkerror( "qmr()", "%s: Wrong number of input arguments: At least %d expected.\n", 10000, "qmr", 2);
assert_checkerror( "qmr(1)", "%s: Wrong number of input arguments: At least %d expected.\n", 10000, "qmr", 2);
assert_checkerror( "qmr([1 2;3 4])", "%s: Wrong number of input arguments: At least %d expected.\n", 10000, "qmr", 2);
assert_checkerror( "qmr([1 2;3 4;5 6],[1;1;1])", "%s: Wrong size for input argument #%d: Square matrix expected.\n", 10000, "qmr", 1);
assert_checkerror( "qmr([1 2;3 4;5 6],[1 1 1])", "%s: Wrong size for input argument #%d: Square matrix expected.\n", 10000, "qmr", 1);
assert_checkerror( "qmr([1 2;3 4],[1 1])", "%s: Wrong size for input argument #%d: Column vector expected.\n", 10000, "qmr", 2);
assert_checkerror( "qmr([%t %t;%t %t],[1;1])", "%s: Wrong type for input argument #%d : A real or complex matrix or a sparse matrix or a function expected.\n", 10000, "qmr", 1);
assert_checkerror( "qmr([],[])", "%s: Wrong size for input argument #%d: Column vector expected.\n", 10000, "qmr", 2);

// 2. Test with 2 input arguments and all output arguments
// =======================================================
A=[10,1;1,10];
b=[11;11];
[xcomputed, flag, err, iter, res]=qmr(A,b);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checkequal ( flag , 0);
assert_checkequal ( err , 0);
assert_checkequal ( iter, 1);

// 3. Test with 3 input arguments and all output arguments
// ======================================================
A=[10,1;1,10];
b=[11;11];
x0=[1;1];
[xcomputed, flag, err, iter, res]=qmr(A,b,x0);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// 4. Test with 4 input arguments and all output arguments
// ======================================================
A=[10,1;1,10];
b=[11;11];
x0=[1;1];
M1=[1,0;0,1];
[xcomputed, flag, err, iter, res]=qmr(A,b,x0,M1);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed,xexpected,%eps);

// 5.Test with 5 input arguments and all output arguments
// ======================================================
A=[10,1;1,10];
b=[11;11];
x0=[1;1];
M1=[1,0;0,1];
M2=[1,0;0,1];
[xcomputed, flag, err, iter, res]=qmr(A,b,x0,M1,M2);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// 6.Test with 6 input arguments and all output arguments
// ======================================================
A=[10,1;1,10];
b=[11;11];
x0 = [1;1];
M1=[1,0;0,1];
M2=[1,0;0,1];
maxit = 10;
[xcomputed, flag, err, iter, res]=qmr(A,b,x0,M1,M2,maxit);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// 7.Test with 7 input arguments and all output arguments
// =======================================================
A=[10,1;1,10];
b=[11;11];
tol = 100*%eps;
maxit = 10;
M1=[1,0;0,1];
M2=[1,0;0,1];
x0=[1;1];
[xcomputed, flag, err, iter, res]=qmr(A,b,x0,M1,M2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// 8. Test with non-positionnal input parameters so that good initialization generates 0 iterations
// =================================================================================================
A=[100,1;1,10.];
b=[101;11];
[xcomputed, flag, err, iter, res]=qmr(A,b,x0=[1.;1.]);
assert_checkequal ( iter , 1 );

// 9. Test the special case where b=0
// ====================================
A=[100,1;1,10.];
b=[0;0];
[xcomputed, flag, err, iter, res]=qmr(A,b);
xexpected=[0;0];
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checkequal ( flag , 0 );
assert_checktrue ( err <= %eps );
assert_checkequal ( iter , 1 );

// 10. A is a sparse matrix
// =========================
A = sparse([10 1;1 10]);
b = ones(2,1);
[xcomputed, flag, err, iter, res]=qmr(A,b);
xexpected=[0.0909091;0.0909091];
assert_checkalmostequal ( xcomputed , xexpected , 1e-1);
assert_checkequal ( flag , 0);
assert_checkalmostequal ( err , 1.110d-16, 1e-1);
assert_checkequal ( iter, 1);

// --------------------
// If A is a function 
// -------------------

// 1. Interface
// =============
function y = func_A(x)
    Amat = [10,1;1,10];
    y = Amat*x;
endfunction
assert_checkfalse(execstr("qmr(func_A)","errcatch") == 0);
lasterror(execstr("10000","errcatch") == 0);

// 2. Test with 2 input arguments and all output arguments
// =======================================================
function y = func_qmr(x,t)
    Amat = [10,1;1,10];
    if (t == 'notransp') then
        y = Amat*x;
    elseif (t == 'transp') then
        y = Amat'*x;
    end
endfunction
b = [11;11];
[xcomputed, flag, err, iter, res]=qmr(func_qmr,b);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checkequal ( flag , 0);
assert_checkequal ( err , 0);
assert_checkequal ( iter, 1);

function y = funcA_notransp(x)
    Amat = [10,1;1,10];
    y = Amat*x;
endfunction

function y = funcA_transp(x)
    Amat = [10,1;1,10];
    y = Amat'*x;
endfunction
b = [11;11];
[xcomputed, flag, err, iter, res]=qmr(funcA_notransp,funcA_transp,b);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checkequal ( flag , 0);
assert_checkequal ( err , 0);
assert_checkequal ( iter, 1);

// 3. Test with 3 input arguments and all output arguments
// =======================================================
function y = func_qmr(x,t)
    Amat = [10,1;1,10];
    if (t == 'notransp') then
        y = Amat*x;
    elseif (t == 'transp') then
        y = Amat'*x;
    end
endfunction

b = [11;11];
x0=[1;1];
[xcomputed, flag, err, iter, res]=qmr(func_qmr,b,x0);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = funcA_notransp(x)
    Amat = [10,1;1,10];
    y = Amat*x;
endfunction

function y = funcA_transp(x)
    Amat = [10,1;1,10];
    y = Amat'*x;
endfunction
b = [11;11];
x0=[1;1];
[xcomputed, flag, err, iter, res]=qmr(funcA_notransp,funcA_transp,b,x0);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// 4. Test with 4 input arguments and all output arguments
// =======================================================
function y = func_qmr(x,t)
    Amat = [10,1;1,10];
    if (t == 'notransp') then
        y = Amat*x;
    elseif (t == 'transp') then
        y = Amat'*x;
    end
endfunction
b = [11;11];
x0 = [1;1];
M1=[1,0;0,1];
[xcomputed, flag, err, iter, res]=qmr(func_qmr,b,x0,M1);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = funcA_notransp(x)
    Amat = [10,1;1,10];
    y = Amat*x;
endfunction

function y = funcA_transp(x)
    Amat = [10,1;1,10];
    y = Amat'*x;
endfunction
b = [11;11];
x0 = [1;1];
M1=[1,0;0,1];
[xcomputed, flag, err, iter, res]=qmr(funcA_notransp,funcA_transp,b,x0,M1);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// 5 Test with 5 input arguments and all output arguments
// =======================================================
function y = func_qmr(x,t)
    Amat = [10,1;1,10];
    if (t == 'notransp') then
        y = Amat*x;
    elseif (t == 'transp') then
        y = Amat'*x;
    end
endfunction
b = [11;11];
x0 = [1;1];
M1=[1,0;0,1];
M2=[1,0;0,1];
[xcomputed, flag, err, iter, res]=qmr(func_qmr,b,x0,M1,M2);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = funcA_notransp(x)
    Amat = [10,1;1,10];
    y = Amat*x;
endfunction

function y = funcA_transp(x)
    Amat = [10,1;1,10];
    y = Amat'*x;
endfunction
b = [11;11];
x0 = [1;1];
M1=[1,0;0,1];
M2 = [1 0; 0 1];
[xcomputed, flag, err, iter, res]=qmr(funcA_notransp,funcA_transp,b,x0,M1,M2);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// 6 Test with 6 input arguments and all output arguments
// =======================================================
function y = func_qmr(x,t)
    Amat = [10,1;1,10];
    if (t == 'notransp') then
        y = Amat*x;
    elseif (t == 'transp') then
        y = Amat'*x;
    end
endfunction
b = [11;11];
x0 = [1;1];
M1=[1,0;0,1];
M2 = eye(2,2);
maxit = 10
[xcomputed, flag, err, iter, res]=qmr(func_qmr,b,x0,M1,M2,maxit);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = funcA_notransp(x)
    Amat = [10,1;1,10];
    y = Amat*x;
endfunction

function y = funcA_transp(x)
    Amat = [10,1;1,10];
    y = Amat'*x;
endfunction
b = [11;11];
x0 = [1;1];
maxit = 10;
M1 = eye(2,2);
M2 = eye(2,2);
[xcomputed, flag, err, iter, res]=qmr(funcA_notransp,funcA_transp,b,x0,M1,M2,maxit);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// 7 Test with 7 input arguments and all output arguments
// =======================================================
function y = func_qmr(x,t)
    Amat = [10,1;1,10];
    if (t == 'notransp') then
        y = Amat*x;
    elseif (t == 'transp') then
        y = Amat'*x;
    end
endfunction
b = [11;11];
tol = 100*%eps;
maxit = 10;
M1=[1,0;0,1];
M2 = eye(2,2);
x0=[1;1];
[xcomputed, flag, err, iter, res]=qmr(func_qmr,b,x0,M1,M2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = funcA_notransp(x)
    Amat = [10,1;1,10];
    y = Amat*x;
endfunction

function y = funcA_transp(x)
    Amat = [10,1;1,10];
    y = Amat'*x;
endfunction
b = [11;11];
tol = 100*%eps;
maxit = 10;
M1 = eye(2,2);
M2 = eye(2,2);
x0 = [1;1];
[xcomputed, flag, err, iter, res]=qmr(funcA_notransp,funcA_transp,b,x0,M1,M2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);


// Test : A is a matrix, M1 is a function, M2 is a matrix (default)
// ===============================================================
A = [10 1;1 10];
b = [11;11];
x0 = [1;1];

function y = funM1(x,t)
    M1 = eye(2,2);
    if (t=="notransp") then
        y = M1*x;
    elseif (t=="transp") then
        y = M1'*x;
    end
endfunction

[xcomputed, flag, err, iter, res]=qmr(A,b,x0,funM1);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = functionM1(x)
    M1 = eye(2,2);
    y = M1*x;
endfunction

function y = functionM1t(x)
    M1 = eye(2,2);
    y = M1'*x;
endfunction

[xcomputed, flag, err, iter, res]=qmr(A,b,x0,functionM1,functionM1t);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// Test : A is a matrix, M1 is a function, M2 is a matrix 
// ======================================================
A = [10 1;1 10];
b = [11;11];
x0 = [1;1];
M2 = eye(2,2);
maxit = 10;
tol = 100*%eps;

function y = funM1(x,t)
    M1 = eye(2,2);
    if (t=="notransp") then
        y = M1*x;
    elseif (t=="transp") then
        y = M1'*x;
    end
endfunction

[xcomputed, flag, err, iter, res]=qmr(A,b,x0,funM1,M2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = functionM1(x)
    M1 = eye(2,2);
    y = M1*x;
endfunction

function y = functionM1t(x)
    M1 = eye(2,2);
    y = M1'*x;
endfunction

[xcomputed, flag, err, iter, res]=qmr(A,b,x0,functionM1,functionM1t,M2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// Test : A is a matrix, M1 is a matrix, M2 is a function
// ======================================================
A = [10 1;1 10];
b = [11;11];
x0 = [1;1];
M1 = eye(2,2);
maxit = 10;
tol = 100*%eps;

function y = funM2(x,t)
    M2 = eye(2,2);
    if (t=="notransp") then
        y = M2*x;
    elseif (t=="transp") then
        y = M2'*x;
    end
endfunction

[xcomputed, flag, err, iter, res]=qmr(A,b,x0,M1,funM2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = functionM2(x)
    M2 = eye(2,2);
    y = M2*x;
endfunction

function y = functionM2t(x)
    M2 = eye(2,2);
    y = M2'*x;
endfunction

[xcomputed, flag, err, iter, res]=qmr(A,b,x0,M1,functionM2,functionM2t,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// Test : A is a matrix, M1 and M2 are functions
// ==============================================
A = [10 1;1 10];
b = [11;11];
x0 = [1;1];
maxit = 10;
tol = 100*%eps;

function y = funM1(x,t)
    M1 = eye(2,2);
    if (t=="notransp") then
        y = M1*x;
    elseif (t=="transp") then
        y = M1'*x;
    end
endfunction

function y = funM2(x,t)
    M2 = eye(2,2);
    if (t=="notransp") then
        y = M2*x;
    elseif (t=="transp") then
        y = M2'*x;
    end
endfunction

[xcomputed, flag, err, iter, res]=qmr(A,b,x0,funM1,funM2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = functionM1(x)
    M1 = eye(2,2);
    y = M1*x;
endfunction

function y = functionM1t(x)
    M1 = eye(2,2);
    y = M1'*x;
endfunction

function y = functionM2(x)
    M2 = eye(2,2);
    y = M2*x;
endfunction

function y = functionM2t(x)
    M2 = eye(2,2);
    y = M2'*x;
endfunction

[xcomputed, flag, err, iter, res]=qmr(A,b,x0,functionM1,functionM1t,functionM2,functionM2t,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

// Test : A, M1 and M2 are functions
// ==================================
b = [11;11];
x0 = [1;1];
maxit = 10;
tol = 100*%eps;

function y = funA(x,t)
    A = [10 1;1 10];
    if (t=="notransp") then
        y = A*x;
    elseif (t=="transp") then
        y = A'*x;
    end
endfunction

function y = funM1(x,t)
    M1 = eye(2,2);
    if (t=="notransp") then
        y = M1*x;
    elseif (t=="transp") then
        y = M1'*x;
    end
endfunction

function y = funM2(x,t)
    M2 = eye(2,2);
    if (t=="notransp") then
        y = M2*x;
    elseif (t=="transp") then
        y = M2'*x;
    end
endfunction

[xcomputed, flag, err, iter, res]=qmr(funA,b,x0,funM1,funM2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

function y = functionA(x)
    A = [10 1;1 10];
    y = A*x;
endfunction

function y = functionAt(x)
    A = [10 1;1 10];
    y = A1*x;
endfunction

function y = functionM1(x)
    M1 = eye(2,2);
    y = M1*x;
endfunction

function y = functionM1t(x)
    M1 = eye(2,2);
    y = M1'*x;
endfunction

function y = functionM2(x)
    M2 = eye(2,2);
    y = M2*x;
endfunction

function y = functionM2t(x)
    M2 = eye(2,2);
    y = M2'*x;
endfunction

[xcomputed, flag, err, iter, res]=qmr(functionA,functionAt,b,x0,functionM1,functionM1t,functionM2,functionM2t,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

[xcomputed, flag, err, iter, res]=qmr(funA,b,x0,functionM1,functionM1t,functionM2,functionM2t,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

[xcomputed, flag, err, iter, res]=qmr(funA,b,x0,funM1,functionM2,functionM2t,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

[xcomputed, flag, err, iter, res]=qmr(functionA,functionAt,b,x0,funM1,functionM2,functionM2t,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

[xcomputed, flag, err, iter, res]=qmr(functionA,functionAt,b,x0,funM1,funM2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

[xcomputed, flag, err, iter, res]=qmr(functionA,functionAt,b,x0,functionM1,functionM1t,funM2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

[xcomputed, flag, err, iter, res]=qmr(funA,b,x0,functionM1,functionM1t,funM2,maxit,tol);
xexpected=[1;1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);

















