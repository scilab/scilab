// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->

r = 2;
R = [1,2;3,4];
R3(:,:,1) = R;
R3(:,:,2) = R';
c = 1 + 2*%i;
C = [1+2*%i,2+4*%i;3+6*%i,4+8*%i];
C3(:,:,1) = C;
C3(:,:,2) = C';


// double + double

//r + r
assert_checkequal(r + r, 4)
//r + c
assert_checkequal(r + c, 3+2*%i)
//c + r
assert_checkequal(c + r, 3+2*%i)
//c + c
assert_checkequal(c + c, 2+4*%i)


//double + DOUBLE

//r + R
assert_checkequal(r + R, [3,4;5,6])
rR3ref(:,:,1) = [3,4;5,6];
rR3ref(:,:,2) = [3,5;4,6];
assert_checkequal(r + R3, rR3ref)
//r + C
assert_checkequal(r + C, [3+2*%i,4+4*%i;5+6*%i,6+8*%i])
rC3ref(:,:,1) = [3+2*%i,4+4*%i;5+6*%i,6+8*%i];
rC3ref(:,:,2) = [3-2*%i,5-6*%i;4-4*%i,6-8*%i];
assert_checkequal(r + C3, rC3ref)
//c + R
assert_checkequal(c + R, [2+2*%i,3+2*%i;4+2*%i,5+2*%i])
cR3ref(:,:,1) = [2+2*%i,3+2*%i;4+2*%i,5+2*%i];
cR3ref(:,:,2) = [2+2*%i,4+2*%i;3+2*%i,5+2*%i];
assert_checkequal(c + R3, cR3ref)
//c + C
assert_checkequal(c + C, [2+4*%i,3+6*%i;4+8*%i,5+10*%i])
cC3ref(:,:,1) = [2+4*%i,3+6*%i;4+8*%i,5+10*%i];
cC3ref(:,:,2) = [2,4-4*%i;3-2*%i,5-6*%i];
assert_checkequal(c + C3, cC3ref)


//DOUBLE + double

//R + r
assert_checkequal(R + r, [3,4;5,6])
R3rref(:,:,1) = [3,4;5,6];
R3rref(:,:,2) = [3,5;4,6];
assert_checkequal(R3 + r, R3rref)
//R + c
assert_checkequal(R + c, [2+2*%i,3+2*%i;4+2*%i,5+2*%i])
R3cref(:,:,1) = [2+2*%i,3+2*%i;4+2*%i,5+2*%i];
R3cref(:,:,2) = [2+2*%i,4+2*%i;3+2*%i,5+2*%i];
assert_checkequal(R3 + c, R3cref)
//C + r
assert_checkequal(C + r, [3+2*%i,4+4*%i;5+6*%i,6+8*%i])
C3rref(:,:,1) = [3+2*%i,4+4*%i;5+6*%i,6+8*%i];
C3rref(:,:,2) = [3-2*%i,5-6*%i;4-4*%i,6-8*%i];
assert_checkequal(C3 + r, C3rref)
//c + c
assert_checkequal(C + c, [2+4*%i,3+6*%i;4+8*%i,5+10*%i])
C3cref(:,:,1) = [2+4*%i,3+6*%i;4+8*%i,5+10*%i];
C3cref(:,:,2) = [2,4-4*%i;3-2*%i,5-6*%i];
assert_checkequal(C3 + c, C3cref)

//DOUBLE + DOUBLE
//R + R
assert_checkequal(R + R, [2,4;6,8])
R3R3ref(:,:,1) = [2,4;6,8];
R3R3ref(:,:,2) = [2,6;4,8];
assert_checkequal(R3 + R3, R3R3ref)
//R + C
assert_checkequal(R + C, [2+2*%i,4+4*%i;6+6*%i,8+8*%i])
R3C3ref(:,:,1) = [2+2*%i,4+4*%i;6+6*%i,8+8*%i];
R3C3ref(:,:,2) = [2-2*%i,6-6*%i;4-4*%i,8-8*%i];
assert_checkequal(R3 + C3, R3C3ref)
//C + R
assert_checkequal(C + R, [2+2*%i,4+4*%i;6+6*%i,8+8*%i])
C3R3ref(:,:,1) = [2+2*%i,4+4*%i;6+6*%i,8+8*%i];
C3R3ref(:,:,2) = [2-2*%i,6-6*%i;4-4*%i,8-8*%i];
assert_checkequal(C3 + R3, C3R3ref)
//C + C
assert_checkequal(C + C, [2+4*%i,4+8*%i;6+12*%i,8+16*%i])
C3C3ref(:,:,1) = [2+4*%i,4+8*%i;6+12*%i,8+16*%i];
C3C3ref(:,:,2) = [2-4*%i,6-12*%i;4-8*%i,8-16*%i];
assert_checkequal(C3 + C3, C3C3ref)
