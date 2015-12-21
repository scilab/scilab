// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

r = 2;
R = [1,2;3,4];
R3(:,:,1) = R;
R3(:,:,2) = R';
c = 1 + 2*%i;
C = [1+2*%i,2+4*%i;3+6*%i,4+8*%i];
C3(:,:,1) = C;
C3(:,:,2) = C';
SP = sparse([1,2;4,5;3,10],[1,2,3]);
SPC = sparse([1,2;4,5;3,10],[1,2,3]) * ( 1 + 4*%i);

//double / double

//r / r
assert_checkalmostequal(r / r, 1);
//r / c
assert_checkalmostequal(r / c, 0.4-0.8*%i);
//c / r
assert_checkalmostequal(c / r, 0.5+%i);
//c / c
assert_checkalmostequal(c / c, 1);



//double / DOUBLE

//r / R
assert_checkalmostequal(r / R, [-4,2;3,-1]);
//r / C
assert_checkalmostequal(r / C, [-0.8+1.6*%i,0.4-0.8*%i;0.6-1.2*%i,-0.2+0.4*%i]);
//c / R
assert_checkalmostequal(c / R, [-2-4*%i,1+2*%i;1.5+3*%i,-0.5-1*%i]);
//c / C
assert_checkalmostequal(c / C, [-2,1;1.5,-0.5]);


//DOUBLE / double

//R / r
assert_checkalmostequal(R / r, [0.5,1;1.5,2]);
R3rref(:,:,1) = [0.5,1;1.5,2];
R3rref(:,:,2) = [0.5,1.5;1,2];
assert_checkalmostequal(R3 / r, R3rref);
//R / c
assert_checkalmostequal(R / c, [0.2-0.4*%i,0.4-0.8*%i;0.6-1.2*%i,0.8-1.6*%i]);
R3cref(:,:,1) = [0.2-0.4*%i,0.4-0.8*%i;0.6-1.2*%i,0.8-1.6*%i];
R3cref(:,:,2) = [0.2-0.4*%i,0.6-1.2*%i;0.4-0.8*%i,0.8-1.6*%i];
assert_checkalmostequal(R3 / c, R3cref);
//C / r
assert_checkalmostequal(C / r, [0.5+1*%i,1+2*%i;1.5+3*%i,2+4*%i]);
C3rref(:,:,1) = [0.5+1*%i,1+2*%i;1.5+3*%i,2+4*%i];
C3rref(:,:,2) = [0.5-1*%i,1.5-3*%i;1-2*%i,2-4*%i];
assert_checkalmostequal(C3 / r, C3rref);
//C / c
assert_checkalmostequal(C / c, [1,2;3,4]);
C3cref(:,:,1) = [1,2;3,4];
C3cref(:,:,2) = [-0.6-0.8*%i,-1.8-2.4*%i;-1.2-1.6*%i,-2.4-3.2*%i];
assert_checkalmostequal(C3 / c, C3cref);


//DOUBLE / DOUBLE

//R / R
assert_checkalmostequal(R / R, [1,0;0,1]);
//R / C
assert_checkalmostequal(R / C, [0.2-0.4*%i,0;0,0.2-0.4*%i]);
//C / R
assert_checkalmostequal(C / R, [1+2*%i,0;0,1+2*%i]);
//C / C
assert_checkalmostequal(C / C, [1,0;0,1]);

//SPARSE / double

//SP / r
assert_checkalmostequal(SP/ r, sparse([1,2;4,5;3,10],[0.5,1,1.5]));
//SP / c
assert_checkalmostequal(SP/ c, sparse([1,2;4,5;3,10],[0.2-0.4*%i,0.4-0.8*%i,0.6-1.2*%i]));
//SPC /r
assert_checkalmostequal(SPC/ r, sparse([1,2;4,5;3,10],[0.5+2*%i,1+4*%i,1.5+6*%i]));
//SPC /c
assert_checkalmostequal(SPC/ c, sparse([1,2;4,5;3,10],[1.8+0.4*%i,3.6+0.8*%i,5.4+1.2*%i]));

