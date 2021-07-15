// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
// Copyright (C) 2019 - 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

r = 2;
R = [1,2;3,4];
c = 1 + 2*%i;
C = [1+2*%i,2+4*%i;3+6*%i,4+8*%i];

// double .*. double
assert_checkequal(r .*. r, 4);
assert_checkequal(r .*. c, 2+4*%i);
assert_checkequal(c .*. r, 2+4*%i);
assert_checkequal(c .*. c, -3+4*%i);

// double .*. DOUBLE
assert_checkequal(r .*. R, [2,4;6,8]);
assert_checkequal(r .*. C, [2+4*%i,4+8*%i;6+12*%i,8+16*%i]);
assert_checkequal(c .*. R, [1+2*%i,2+4*%i;3+6*%i,4+8*%i]);
assert_checkequal(c .*. C, [-3+4*%i,-6+8*%i;-9+12*%i,-12+16*%i]);

// DOUBLE .*. double
assert_checkequal(R .*.r, [2,4;6,8]);
assert_checkequal(R .*.c, [1+2*%i,2+4*%i;3+6*%i,4+8*%i]);
assert_checkequal(C .*.r, [2+4*%i,4+8*%i;6+12*%i,8+16*%i]);
assert_checkequal(C .*.c, [-3+4*%i,-6+8*%i;-9+12*%i,-12+16*%i]);

// DOUBLE .*. DOUBLE
assert_checkequal(R .*. R, [1,2,2,4;3,4,6,8;3,6,4,8;9,12,12,16]);
assert_checkequal(R .*. C, [1+2*%i,2+4*%i,2+4*%i,4+8*%i;3+6*%i,4+8*%i,6+12*%i,8+16*%i;3+6*%i,6+12*%i,4+8*%i,8+16*%i;9+18*%i,12+24*%i,12+24*%i,16+32*%i]);
assert_checkequal(C .*. R, [1+2*%i,2+4*%i,2+4*%i,4+8*%i;3+6*%i,4+8*%i,6+12*%i,8+16*%i;3+6*%i,6+12*%i,4+8*%i,8+16*%i;9+18*%i,12+24*%i,12+24*%i,16+32*%i]);
assert_checkequal(C .*. C, [-3+4*%i,-6+8*%i,-6+8*%i,-12+16*%i;-9+12*%i,-12+16*%i,-18+24*%i,-24+32*%i;-9+12*%i,-18+24*%i,-12+16*%i,-24+32*%i;-27+36*%i,-36+48*%i,-36+48*%i,-48+64*%i]);


// With hypermatrices
// ------------------
r = [1 2 3];
c = [1 2 3]';
m = [1 2 ; 3 4];
h = cat(3,[1 2],[3 4]);
assert_checkequal(r.*.ones(1,2,2), cat(3,[1 1 2 2 3 3],[1 1 2 2 3 3]));
assert_checkequal(c.*.ones(1,2,2), cat(3,[1 1;2 2;3 3],[1 1;2 2;3 3]));
assert_checkequal(m.*.ones(1,2,2), cat(3,[1 1 2 2;3 3 4 4],[1 1 2 2;3 3 4 4]));
assert_checkequal(h.*.ones(1,2,2), cat(3,[1 1 2 2],[1 1 2 2],[3 3 4 4],[3 3 4 4]));

assert_checkequal(ones(1,2,2).*.r, cat(3,[1 2 3 1 2 3],[1 2 3 1 2 3]));
assert_checkequal(ones(1,2,2).*.c, cat(3,[1 1;2 2;3 3],[1 1;2 2;3 3]));
assert_checkequal(ones(1,2,2).*.m, cat(3,[1 2 1 2;3 4 3 4],[1 2 1 2;3 4 3 4]));
assert_checkequal(ones(1,2,2).*.h, cat(3,[1 2 1 2],[3 4 3 4],[1 2 1 2],[3 4 3 4]));

assert_checkequal(size(rand(2,3,4,5).*.rand(3,1,1,1,1,7)), [6 3 4 5 1 7]);


// BOOLEANS .*. BOOLEANS
// ---------------------
b = [%T %F %T]; spb = sparse([%F %T %T]');
assert_checkequal(b.*.b', [1 0 1 ; 0 0 0 ; 1 0 1]);
assert_checkequal(b.*.spb, sparse([0 0 0 ; 1 0 1 ; 1 0 1]));
assert_checkequal(spb.*.b, sparse([0 0 0 ; 1 0 1 ; 1 0 1]));
assert_checkequal(spb.*.spb', sparse([0 0 0 ; 0 1 1 ; 0 1 1]));

assert_checkequal(kron(b, b'), [1 0 1 ; 0 0 0 ; 1 0 1]);
assert_checkequal(kron(b, spb), sparse([0 0 0 ; 1 0 1 ; 1 0 1]));
assert_checkequal(kron(spb, b), sparse([0 0 0 ; 1 0 1 ; 1 0 1]));
assert_checkequal(kron(spb, spb'), sparse([0 0 0 ; 0 1 1 ; 0 1 1]));


// BOOLEANS .*. DOUBLES
// --------------------
b = [%T %F %T]; spb = sparse([%F %T %T]');
m = [-1 2 %inf]; sp = sparse(m');
assert_checkequal(b .*. m', [-1 0 -1 ; 2 0 2 ; %inf %nan %inf]);
assert_checkequal(b .*. sp, sparse([-1 0 -1 ; 2 0 2 ; %inf 0 %inf]));   // 0 instead of %nan
assert_checkequal(spb .*. m, sparse([0 0 0 ; -1 2 %inf ; -1 2 %inf]));
assert_checkequal(spb .*. sp', sparse([0 0 0 ; -1 2 %inf ; -1 2 %inf]));

assert_checkequal(kron(b, m'), [-1 0 -1 ; 2 0 2 ; %inf %nan %inf]);
assert_checkequal(kron(b, sp), sparse([-1 0 -1 ; 2 0 2 ; %inf 0 %inf]));
assert_checkequal(kron(spb, m), sparse([0 0 0 ; -1 2 %inf ; -1 2 %inf]));
assert_checkequal(kron(spb, sp'), sparse([0 0 0 ; -1 2 %inf ; -1 2 %inf]));


// DOUBLES .*. BOOLEANS
// --------------------
b = [%T %F %T]; spb = sparse([%F %T %T]');
m = [-1 2 %inf]; sp = sparse(m');
assert_checkequal(m .*. b', [-1 2 %inf ; 0 0 %nan ; -1 2 %inf]);
assert_checkequal(m .*. spb, sparse([0 0 0 ; -1 2 %inf ; -1 2 %inf]));
assert_checkequal(sp .*. b, sparse([-1 0 -1 ; 2 0 2 ; %inf 0 %inf]));
assert_checkequal(sp .*. spb', sparse([0 -1 -1 ; 0 2 2 ; 0 %inf %inf]));

assert_checkequal(kron(m, b'), [-1 2 %inf ; 0 0 %nan ; -1 2 %inf]);
assert_checkequal(kron(m, spb), sparse([0 0 0 ; -1 2 %inf ; -1 2 %inf]));
assert_checkequal(kron(sp, b), sparse([-1 0 -1 ; 2 0 2 ; %inf 0 %inf]));
assert_checkequal(kron(sp, spb'), sparse([0 -1 -1 ; 0 2 2 ; 0 %inf %inf]));


// BOOLEANS, INTEGERS
// ------------------
b = [%F %T %T]; i = int8([-2 1 3]');
assert_checkequal(b .*. i, int8([0 -2 -2 ; 0 1 1 ; 0 3 3]));
assert_checkequal(i' .*. b, int8([0 -2 -2  0  1  1  0  3  3]));
assert_checkequal(kron(b, i), int8([0 -2 -2 ; 0 1 1 ; 0 3 3]));
assert_checkequal(kron(i', b), int8([0 -2 -2  0  1  1  0  3  3]));

// BOOLEANS, POLYNOMIALS
// ---------------------
s = %s; b = [%T %T %F]'; p = (1-s).^(0:2);
Ref = [1,1-s,1-2*s+s^2 ; 1,1-s,1-2*s+s^2 ; 0, 0, 0];
assert_checkequal(b .*. p, Ref);
assert_checkequal(p .*. b, Ref);
assert_checkequal(kron(b, p), Ref);
assert_checkequal(kron(p, b), Ref);

// BOOLEANS, RATIONALS
// -------------------
s = %s; b = [%T %F %T]'; r = (1-s).^(0:2)/s;
Ref = [1,1-s,1-2*s+s^2 ; 0 0 0 ; 1,1-s,1-2*s+s^2]/s;
assert_checkequal(b .*. r, Ref);
assert_checkequal(r .*. b, Ref);
assert_checkequal(kron(b, r), Ref);
assert_checkequal(kron(r, b), Ref);
