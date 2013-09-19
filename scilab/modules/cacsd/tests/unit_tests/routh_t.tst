// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for routh_t() function
// =============================================================================

s=poly(0,'s');
k=poly(0,'k');

//Test with a polynomial
P1=s*(s+7)*(s+11);
assert_checkequal(routh_t(P1), [1 77; 18 0; 77 0; 0 0]);
[r, num] = routh_t(P1);
assert_checkequal(r, [1 77; 18 0; 77 0; 0 0]);
assert_checkequal(num, 0);

//Test with a transfert function
h=1/P1;
ref1=[1 77; 18 k;(1386-k)/18 0; k 0];
numref1=numer(ref1);
numref1(3)=1386 - k;
denref1=denom(ref1);
denref1(3)=18;
assert_checkequal(numer(routh_t(h,k))/horner(denom(routh_t(h,k)),1),numref1/denref1);
assert_checkequal(numer(routh_t(h,k,%t))/horner(denom(routh_t(h,k,%t)),1),numref1/denref1);
assert_checkequal(routh_t(h,k,%f),[1 77; 18 k; 1386-k 0; 1386*k-k*k 0]);

// One of the coefficients in the polynomial equals zero
P2=2*s^2-24;
assert_checkequal(routh_t(P2), [2 -24; 4 0; -24 0]);
[r, num] = routh_t(P2);
assert_checkequal(r, [2 -24; 4 0; -24 0]);
assert_checkequal(num, 1);

// A row full of zeros
P3=s^4-6*s^3+10*s^2-6*s+9;
assert_checkequal(routh_t(P3),[1 10 9; -6 -6 0; 9 9 0; 18 0 0; 9 0 0]);
[r, num] = routh_t(P3);
assert_checkequal(r,[1 10 9; -6 -6 0; 9 9 0; 18 0 0; 9 0 0]);
assert_checkequal(num, 2);

//Error messages
assert_checkfalse(execstr("routh_t(P,%t)","errcatch")==0);
assert_checkfalse(execstr("routh_t(P,k)","errcatch")==0);
assert_checkfalse(execstr("routh_t()","errcatch")==0);
assert_checkfalse(execstr("routh_t(h)","errcatch")==0);
assert_checkfalse(execstr("routh_t(h,k,%t,%t)","errcatch")==0);
