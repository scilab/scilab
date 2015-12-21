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


// double * double

// r * r
assert_checkequal(r * r, 4);
// r * c
assert_checkequal(r * c, 2+%i*4);
// c * r
assert_checkequal(c * r, 2+%i*4);
// c * c
assert_checkequal(c * c, -3+%i*4);


// double * DOUBLE

// r * R
assert_checkequal(r * R, [2,4;6,8]);
rR3ref(:,:,1) = [2,4;6,8];
rR3ref(:,:,2) = [2,6;4,8];
assert_checkequal(r * R3, rR3ref);
// r * C
assert_checkequal(r * C, [2+%i*4,4+%i*8;6+%i*12,8+%i*16]);
rC3ref(:,:,1) = [2+%i*4, 4+%i*8; 6+%i*12, 8+%i*16];
rC3ref(:,:,2) = [2-%i*4, 6-%i*12; 4-%i*8, 8-%i*16];
assert_checkequal(r * C3, rC3ref);
// c * R
assert_checkequal(c * R, [1+%i*2,2+%i*4;3+%i*6,4+%i*8]);
cR3ref(:,:,1) = [1+%i*2, 2+%i*4; 3+%i*6, 4+%i*8];
cR3ref(:,:,2) = [1+%i*2, 3+%i*6; 2+%i*4, 4+%i*8];
assert_checkequal(c * R3, cR3ref);
// c * C
assert_checkequal(c * C, [-3+%i*4,-6+%i*8;-9+%i*12,-12+%i*16]);
cC3ref(:,:,1) = [-3+%i*4,-6+%i*8;-9+%i*12,-12+%i*16];
cC3ref(:,:,2) = [ 5, 15; 10, 20];
assert_checkequal(c * C3, cC3ref);


// DOUBLE * double

// R * r
assert_checkequal(R * r, [2,4;6,8]);
R3rref(:,:,1) = [2,4;6,8];
R3rref(:,:,2) = [2,6;4,8];
assert_checkequal(R3 * r, R3rref);
// R * c
assert_checkequal(R * c, [ 1+%i*2, 2+%i*4; 3+%i*6, 4+%i*8]);
R3cref(:,:,1) = [ 1+%i*2, 2+%i*4; 3+%i*6, 4+%i*8];
R3cref(:,:,2) = [ 1+%i*2, 3+%i*6; 2+%i*4, 4+%i*8];
assert_checkequal(R3 * c, R3cref);
// C * r
assert_checkequal(C * r, [ 2+%i*4, 4+%i*8; 6+%i*12, 8+%i*16]);
C3rref(:,:,1) = [ 2+%i*4, 4+%i*8; 6+%i*12, 8+%i*16];
C3rref(:,:,2) = [ 2-%i*4, 6-%i*12; 4-%i*8, 8-%i*16];
assert_checkequal(C3 * r, C3rref);
// C * c
assert_checkequal(C * c, [-3+%i*4,-6+%i*8;-9+%i*12,-12+%i*16]);
C3cref(:,:,1) = [-3+%i*4,-6+%i*8;-9+%i*12,-12+%i*16];
C3cref(:,:,2) = [ 5, 15; 10, 20];
assert_checkequal(C3 * c, C3cref);


// DOUBLE * DOUBLE

// R * R
assert_checkequal(R * R, [7,10;15,22]);
// R * C
assert_checkequal(R * C, [ 7+%i*14, 10+%i*20; 15+%i*30, 22+%i*44]);
// C * R
assert_checkequal(C * R, [ 7+%i*14, 10+%i*20; 15+%i*30, 22+%i*44]);
// C * C
assert_checkequal(C * C, [-21+%i*28,-30+%i*40;-45+%i*60,-66+%i*88]);
