// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

r = 2;
R = [1,2;3,4];
c = 1 + 2*%i;
C = [1+2*%i,2+4*%i;3+6*%i,4+8*%i];

// double .*. double

//r .*. r
assert_checkequal(r .*. r, 4);
//r .*. c
assert_checkequal(r .*. c, 2+4*%i);
//c .*. r
assert_checkequal(c .*. r, 2+4*%i);
//c .*. c
assert_checkequal(c .*. c, -3+4*%i);


// double .*. DOUBLE

//r .*. R
assert_checkequal(r .*. R, [2,4;6,8]);
//r .*. C
assert_checkequal(r .*. C, [2+4*%i,4+8*%i;6+12*%i,8+16*%i]);
//c .*. R
assert_checkequal(c .*. R, [1+2*%i,2+4*%i;3+6*%i,4+8*%i]);
//c .*. C
assert_checkequal(c .*. C, [-3+4*%i,-6+8*%i;-9+12*%i,-12+16*%i]);


// DOUBLE .*. double

//R .*. r
assert_checkequal(R .*.r, [2,4;6,8]);
//R .*. c
assert_checkequal(R .*.c, [1+2*%i,2+4*%i;3+6*%i,4+8*%i]);
//C .*. r
assert_checkequal(C .*.r, [2+4*%i,4+8*%i;6+12*%i,8+16*%i]);
//C .*. c
assert_checkequal(C .*.c, [-3+4*%i,-6+8*%i;-9+12*%i,-12+16*%i]);


// DOUBLE .*. DOUBLE

//R .*. R
assert_checkequal(R .*. R, [1,2,2,4;3,4,6,8;3,6,4,8;9,12,12,16]);
//R .*. C
assert_checkequal(R .*. C, [1+2*%i,2+4*%i,2+4*%i,4+8*%i;3+6*%i,4+8*%i,6+12*%i,8+16*%i;3+6*%i,6+12*%i,4+8*%i,8+16*%i;9+18*%i,12+24*%i,12+24*%i,16+32*%i]);
//C .*. R
assert_checkequal(C .*. R, [1+2*%i,2+4*%i,2+4*%i,4+8*%i;3+6*%i,4+8*%i,6+12*%i,8+16*%i;3+6*%i,6+12*%i,4+8*%i,8+16*%i;9+18*%i,12+24*%i,12+24*%i,16+32*%i]);
//C .*. C
assert_checkequal(C .*. C, [-3+4*%i,-6+8*%i,-6+8*%i,-12+16*%i;-9+12*%i,-12+16*%i,-18+24*%i,-24+32*%i;-9+12*%i,-18+24*%i,-12+16*%i,-24+32*%i;-27+36*%i,-36+48*%i,-36+48*%i,-48+64*%i]);
