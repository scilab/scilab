// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

//-------------------------------------------------------
// PCG

// Test with 2 input arguments and all output arguments
A = [10 1; 1 10];
b = [11; 11];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "pcg");
xexpected = [1; 1];
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checkequal ( flag , 0);
assert_checkequal ( err , 0);
assert_checkequal ( iter, 1);
// Test with 3 input arguments and all output arguments
tol = 100*%eps;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "pcg", tol);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checktrue ( err <= tol);
// Test with 4 input arguments and all output arguments
maxit = 10;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "pcg", tol, maxit);
assert_checkalmostequal ( xcomputed,xexpected,%eps);
// Test with 5 input arguments and all output arguments
M1 = [1 0; 0 1];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "pcg", tol, maxit, M1);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with 6 input arguments and all output arguments
M2 = M1;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "pcg", tol, maxit, M1, M2);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with 7 input arguments and all output arguments
x0 = [1; 1];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "pcg", tol, maxit, M1, M2, x0);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with non-positional input parameters so that 0 iteration can happen
A2 = [100 1; 1 10];
b2 = [101; 11];
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2,  method="pcg", maxIter=0);
assert_checkequal ( iter , 0 );
// Test with non-positional input parameters so that 1 iteration is sufficient
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2,  method="pcg", tol=0.1);
assert_checkequal ( iter , 1 );
// Test with non-positional input parameters so that pre-conditionning is necessary
A3 = [100 1; 1 0.0101];
M = A3;
[xcomputed, flag, err, iter, res] = conjgrad(A3, b2, method="pcg", tol, maxit,%M=M, maxIter=5, tol=%eps);
assert_checkequal ( flag , 0 );
// Test with non-positional input parameters so that good initialization generates 0 iterations
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2, "pcg", x0=[1;1]);
assert_checkequal ( iter , 0 );
// Test the special case where b = 0
b3 = [0; 0];
[xcomputed, flag, err, iter, res] = conjgrad(A2, b3, "pcg");
xexpected2 = [0; 0];
assert_checkalmostequal ( xcomputed , xexpected2 , %eps);
assert_checkequal ( flag , 0 );
assert_checktrue ( err <= %eps );
assert_checkequal ( iter , 0 );
// Try a hard case where preconditionning is necessary
// This is the Hilbert 5x5 matrix : A = 1/(testmatrix("hilb",5))
A4 = [
1.                          0.5000000000000001110223    0.3333333333333334258519    0.2500000000000000555112    0.2000000000000000666134
0.4999999999999982236432    0.3333333333333319825620    0.2499999999999988897770    0.1999999999999990951682    0.1666666666666659080143
0.3333333333333320380731    0.2499999999999990563104    0.1999999999999992617017    0.1666666666666660745477    0.1428571428571423496123
0.2499999999999990285549    0.1999999999999993449684    0.1666666666666661855700    0.1428571428571424883902    0.1249999999999996808109
0.1999999999999991506794    0.1666666666666660745477    0.1428571428571424328791    0.1249999999999996669331    0.11111111111111082739
];
b4 = ones(5, 1);
M = A4;
[xcomputed, flag, err, iter, res] = conjgrad(A4, b4, "pcg", %M=M, tol=%eps, maxIter=3);
expected = [
5
-120
630
-1120
630 ];
assert_checkalmostequal ( xcomputed , expected, 1.e8*%eps );
assert_checkequal ( flag , 0 );
assert_checkequal ( iter , 2 );
// Try a difficult case where preconditionning is necessary
// Use two pre-conditionning matrices.
// This is the Cholesky factorization of the matrix A : C = chol(A)
C = [
1.    0.5000000000000001110223    0.3333333333333334258519    0.2500000000000000555112    0.2000000000000000666134
0.    0.288675134594810367528     0.2886751345948112557060    0.2598076211353305131624    0.2309401076758494653074
0.    0.                          0.0745355992499937836104    0.1118033988749897594817    0.1277753129999876502421
0.    0.                          0.                          0.0188982236504644136865    0.0377964473009222076683
0.    0.                          0.                          0.                          0.0047619047619250291087
];
M1 = C';
M2 = C;
[xcomputed, flag, err, iter, res] = conjgrad(A4, b4, "pcg", %M=M1, %M2=M2, tol=%eps, maxIter=10);
assert_checkalmostequal ( xcomputed , expected, 1.e8*%eps );
assert_checkequal ( flag , 0 );
assert_checkequal ( iter , 2 );
// Well-conditionned problem
A5 = [ 94  0   0   0    0   28  0   0   32  0
0   59  13  5    0   0   0   10  0   0
0   13  72  34   2   0   0   0   0   65
0   5   34  114  0   0   0   0   0   55
0   0   2   0    70  0   28  32  12  0
28  0   0   0    0   87  20  0   33  0
0   0   0   0    28  20  71  39  0   0
0   10  0   0    32  0   39  46  8   0
32  0   0   0    12  33  0   8   82  11
0   0   65  55   0   0   0   0   11  100];
b5 = ones(10, 1);
[x, fail, err, iter, res] = conjgrad(A5, b5, "pcg", 1d-12, 10);
assert_checkequal ( flag ,  0 );
assert_checkequal ( iter , 10 );

//-------------------------------------------------------
// CGS

// Test with 2 input arguments and all output arguments
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "cgs");
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checkequal ( flag , 0);
assert_checkequal ( err , 0);
assert_checkequal ( iter, 1);
// Test with 3 input arguments and all output arguments
tol = 100*%eps;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "cgs", tol);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checktrue ( err <= tol);
// Test with 4 input arguments and all output arguments
maxit = 10;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "cgs", tol, maxit);
assert_checkalmostequal ( xcomputed,xexpected,%eps);
// Test with 5 input arguments and all output arguments
M1 = [1 0; 0 1];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "cgs", tol, maxit, M1);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with 6 input arguments and all output arguments
M2 = M1;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "cgs", tol, maxit, M1, M2);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with 7 input arguments and all output arguments
x0 = [1; 1];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "cgs", tol, maxit, M1, M2, x0);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with non-positional input parameters so that 0 iteration can happen
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2,  method="cgs", maxIter=0);
assert_checkequal ( iter , 0 );
// Test with non-positional input parameters so that 1 iteration is sufficient
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2,  method="cgs", tol=0.1);
assert_checkequal ( iter , 1 );
// Test with non-positional input parameters so that pre-conditionning is necessary
M = A3;
[xcomputed, flag, err, iter, res] = conjgrad(A3, b2, method="cgs", tol, maxit,%M=M, maxIter=5, tol=%eps);
assert_checkequal ( flag , 0 );
// Test with non-positional input parameters so that good initialization generates 0 iterations
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2, "cgs", x0=[1;1]);
assert_checkequal ( iter , 0 );
// Test the special case where b = 0
[xcomputed, flag, err, iter, res] = conjgrad(A2, b3, "cgs");
xexpected2 = [0; 0];
assert_checkalmostequal ( xcomputed , xexpected2 , %eps);
assert_checkequal ( flag , 0 );
assert_checktrue ( err <= %eps );
assert_checkequal ( iter , 0 );
// Try a hard case where preconditionning is necessary
// This is the Hilbert 5x5 matrix : A = 1/(testmatrix("hilb",5))
M = A4;
[xcomputed, flag, err, iter, res] = conjgrad(A4, b4, "cgs", %M=M, tol=%eps, maxIter=3);
assert_checkalmostequal ( xcomputed , expected, 1.e8*%eps );
assert_checkequal ( flag , 0 );
assert_checkequal ( iter , 2 );
// Try a difficult case where preconditionning is necessary
// Use two pre-conditionning matrices.
// This is the Cholesky factorization of the matrix A : C = chol(A)
M1 = C';
M2 = C;
[xcomputed, flag, err, iter, res] = conjgrad(A4, b4, "cgs", %M=M1, %M2=M2, tol=%eps, maxIter=10);
assert_checkalmostequal ( xcomputed , expected, 1.e8*%eps );
assert_checkequal ( flag , 0 );
assert_checkequal ( iter , 10 );
// Well-conditionned problem with a nonsymmetric matrix (top-right element)
A5 = [ 94  0   0   0    0   28  0   0   32  20
0   59  13  5    0   0   0   10  0   0
0   13  72  34   2   0   0   0   0   65
0   5   34  114  0   0   0   0   0   55
0   0   2   0    70  0   28  32  12  0
28  0   0   0    0   87  20  0   33  0
0   0   0   0    28  20  71  39  0   0
0   10  0   0    32  0   39  46  8   0
32  0   0   0    12  33  0   8   82  11
0   0   65  55   0   0   0   0   11  100];
[x, fail, err, iter, res] = conjgrad(A5, b5, "cgs", 1d-12, 15);
assert_checkequal ( flag ,  0 );
assert_checkequal ( iter , 11 );

//-------------------------------------------------------
// BICG

// Test with 2 input arguments and all output arguments
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicg");
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checkequal ( flag , 0);
assert_checkequal ( err , 0);
assert_checkequal ( iter, 1);
// Test with 3 input arguments and all output arguments
tol = 100*%eps;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicg", tol);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checktrue ( err <= tol);
// Test with 4 input arguments and all output arguments
maxit = 10;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicg", tol, maxit);
assert_checkalmostequal ( xcomputed,xexpected,%eps);
// Test with 5 input arguments and all output arguments
M1 = [1 0; 0 1];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicg", tol, maxit, M1);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with 6 input arguments and all output arguments
M2 = M1;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicg", tol, maxit, M1, M2);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with 7 input arguments and all output arguments
x0 = [1; 1];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicg", tol, maxit, M1, M2, x0);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with non-positional input parameters so that 0 iteration can happen
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2,  method="bicg", maxIter=0);
assert_checkequal ( iter , 0 );
// Test with non-positional input parameters so that 1 iteration is sufficient
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2,  method="bicg", tol=0.1);
assert_checkequal ( iter , 1 );
// Test with non-positional input parameters so that pre-conditionning is necessary
M = A3;
[xcomputed, flag, err, iter, res] = conjgrad(A3, b2, method="bicg", tol, maxit,%M=M, maxIter=5, tol=%eps);
assert_checkequal ( flag , 0 );
// Test with non-positional input parameters so that good initialization generates 0 iterations
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2, "bicg", x0=[1;1]);
assert_checkequal ( iter , 0 );
// Test the special case where b = 0
[xcomputed, flag, err, iter, res] = conjgrad(A2, b3, "bicg");
assert_checkalmostequal ( xcomputed , xexpected2 , %eps);
assert_checkequal ( flag , 0 );
assert_checktrue ( err <= %eps );
assert_checkequal ( iter , 0 );
// Try a hard case where preconditionning is necessary
// This is the Hilbert 5x5 matrix : A = 1/(testmatrix("hilb",5))
M = A4;
[xcomputed, flag, err, iter, res] = conjgrad(A4, b4, "bicg", %M=M, tol=%eps, maxIter=3);
assert_checkalmostequal ( xcomputed , expected, 1.e8*%eps );
assert_checkequal ( flag , 0 );
assert_checkequal ( iter , 2 );
// Try a difficult case where preconditionning is necessary
// Use two pre-conditionning matrices.
// This is the Cholesky factorization of the matrix A : C = chol(A)
M1 = C';
M2 = C;
[xcomputed, flag, err, iter, res] = conjgrad(A4, b4, "bicg", %M=M1, %M2=M2, tol=%eps, maxIter=10);
assert_checkalmostequal ( xcomputed , expected, 1.e8*%eps );
assert_checkequal ( flag , 0 );
assert_checkequal ( iter , 8 );
// Well-conditionned problem with a nonsymmetric matrix (top-right element)
[x, fail, err, iter, res] = conjgrad(A5, b5, "bicg", 1d-12, 15);
assert_checkequal ( flag ,  0 );
assert_checkequal ( iter , 10 );

//-------------------------------------------------------
// BICGSTAB

// Test with 2 input arguments and all output arguments
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicgstab");
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checkequal ( flag , 0);
assert_checkequal ( err , 0);
assert_checkequal ( iter, 1);
// Test with 3 input arguments and all output arguments
tol = 100*%eps;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicgstab", tol);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
assert_checktrue ( err <= tol);
// Test with 4 input arguments and all output arguments
maxit = 10;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicgstab", tol, maxit);
assert_checkalmostequal ( xcomputed,xexpected,%eps);
// Test with 5 input arguments and all output arguments
M1 = [1 0; 0 1];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicgstab", tol, maxit, M1);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with 6 input arguments and all output arguments
M2 = M1;
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicgstab", tol, maxit, M1, M2);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with 7 input arguments and all output arguments
x0 = [1; 1];
[xcomputed, flag, err, iter, res] = conjgrad(A, b, "bicgstab", tol, maxit, M1, M2, x0);
assert_checkalmostequal ( xcomputed , xexpected , %eps);
// Test with non-positional input parameters so that 0 iteration can happen
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2,  method="bicgstab", maxIter=0);
assert_checkequal ( iter , 0 );
// Test with non-positional input parameters so that 1 iteration is sufficient
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2,  method="bicgstab", tol=0.1);
assert_checkequal ( iter , 1 );
// Test with non-positional input parameters so that pre-conditionning is necessary
M = A3;
[xcomputed, flag, err, iter, res] = conjgrad(A3, b2, method="bicgstab", tol, maxit,%M=M, maxIter=5, tol=%eps);
assert_checkequal ( flag , 0 );
// Test with non-positional input parameters so that good initialization generates 0 iterations
[xcomputed, flag, err, iter, res] = conjgrad(A2, b2, "bicgstab", x0=[1;1]);
assert_checkequal ( iter , 0 );
// Test the special case where b = 0
[xcomputed, flag, err, iter, res] = conjgrad(A2, b3, "bicgstab");
assert_checkalmostequal ( xcomputed , xexpected2 , %eps);
assert_checkequal ( flag , 0 );
assert_checktrue ( err <= %eps );
assert_checkequal ( iter , 0 );
// Try a hard case where preconditionning is necessary
// This is the Hilbert 5x5 matrix : A = 1/(testmatrix("hilb",5))
M = A4;
[xcomputed, flag, err, iter, res] = conjgrad(A4, b4, "bicgstab", %M=M, tol=%eps, maxIter=3);
assert_checkalmostequal ( xcomputed , expected, 1.e8*%eps );
assert_checkequal ( flag , 0 );
assert_checkequal ( iter , 1 );
// Try a difficult case where preconditionning is necessary
// Use two pre-conditionning matrices.
// This is the Cholesky factorization of the matrix A : C = chol(A)
M1 = C';
M2 = C;
[xcomputed, flag, err, iter, res] = conjgrad(A4, b4, "bicgstab", %M=M1, %M2=M2, tol=%eps, maxIter=10);
assert_checkalmostequal ( xcomputed , expected, 1.e8*%eps );
assert_checkequal ( flag , 0 );
assert_checkequal ( iter , 9 );
// Well-conditionned problem with a nonsymmetric matrix (top-right element)
[x, fail, err, iter, res] = conjgrad(A5, b5, "bicgstab", 1d-12, 15);
assert_checkequal ( flag ,  0 );
assert_checkequal ( iter , 10 );



//-------------------------------------------------------
// Error checks

refMsg = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),"conjgrad",2,9);
assert_checkerror("conjgrad(A);", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d.\n"),"conjgrad",1);
assert_checkerror("conjgrad(""string"", b);", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"conjgrad",1);
assert_checkerror("conjgrad(ones(5, 4), b);", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: A matrix expected.\n"),"conjgrad",2);
assert_checkerror("conjgrad(A, ""string"");", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: Column vector expected.\n"),"conjgrad",2);
assert_checkerror("conjgrad(A, A);", refMsg);

refMsg = msprintf(_("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"conjgrad",2,1);
assert_checkerror("conjgrad(A, b4);", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: Single String expected.\n"),"conjgrad",3);
assert_checkerror("conjgrad(A, b, 1d-12, 15);", refMsg);

refMsg = msprintf(_("%s: Wrong value for input argument #%d: %s, %s, %s or %s expected.\n"),"conjgrad",3,"pcg","cgs","bicg","bicgstab");
assert_checkerror("conjgrad(A, b, ""gmres"", 1d-12, 15);", refMsg);
