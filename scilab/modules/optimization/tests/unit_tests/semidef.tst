// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2011 - DIGITEO - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// 1. Define the initial guess
x0=[0;0];
//
// 2. Create a compressed representation of F
// Define 3 symmetric block-diagonal matrices: F0, F1, F2
F0=[2,1,0,0;
    1,2,0,0;
    0,0,3,1;
    0,0,1,3];
F1=[1,2,0,0;
    2,1,0,0;
    0,0,1,3;
    0,0,3,1];
F2=[2,2,0,0;
    2,2,0,0;
    0,0,3,4;
    0,0,4,4];
// Define the size of the two blocks:
// the first block has size 2, 
// the second block has size 2.
blocksizes=[2,2];
// Extract the two blocks of the matrices.
F01=F0(1:2,1:2);
F02=F0(3:4,3:4);
F11=F1(1:2,1:2);
F12=F1(3:4,3:4);
F21=F2(1:2,1:2);
F22=F2(3:4,3:4);
// Create 3 column vectors, containing nonzero entries 
// in F0, F1, F2.
F0nnz = list2vec(list(F01,F02));
F1nnz = list2vec(list(F11,F12));
F2nnz = list2vec(list(F21,F22));
// Create a 16-by-3 matrix, representing the 
// nonzero entries of the 3 matrices F0, F1, F2.
FF=[F0nnz,F1nnz,F2nnz];
// Compress FF
CFF = pack(FF,blocksizes);
//
// 3. Create a compressed representation of Z
// Create the matrix Z0
Z0=2*F0;
// Extract the two blocks of the matrix
Z01=Z0(1:2,1:2);
Z02=Z0(3:4,3:4);
// Create 2 column vectors, containing nonzero entries 
// in Z0.
ZZ0 = [Z01(:);Z02(:)];
// Compress ZZO
CZZ0 = pack(ZZ0,blocksizes);
//
// 4. Create the linear vector c
c=[trace(F1*Z0);trace(F2*Z0)];
// 
// 5. Define the algorithm options
nu = 10;
abstol=1.d-8;
reltol=1.d-10;
tv = 0;
maxiters = 50;
options=[nu,abstol,reltol,tv,maxiters];
// 6. Solve the problem
[x,CZ,ul,info]=semidef(x0,CZZ0,CFF,blocksizes,c,options);
//
// 7. Check the output
// Unpack CZ
Z=unpack(CZ,blocksizes);
w=vec2list(Z,[blocksizes;blocksizes]);

Z=sysdiag(w(1),w(2));
// Check that info states that absolute convergence occurred
assert_checkequal(info(1),2);
// Check that ul contains the expected values
expectedUl1=c'*x;
assert_checkalmostequal(expectedUl1,ul(1),100*%eps);
expectedUl2=-trace(F0*Z);
assert_checkalmostequal(expectedUl2,ul(2),100*%eps);
// Check Slater's condition, which states that the duality gap is zero at optimum.
dualitygap = ul(1)-ul(2);
assert_checktrue(dualitygap < abstol);

