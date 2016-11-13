// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Pierre-Aime AGNEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14476 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14476
//
// <-- Short Description -->
// Polynomial * complex always returned 0

// Real polynomial
ReP = (1 - %z)^3;
// Imaginary polynomial
ImP = 2*%i + %i * %z;
// Real number
ReD = 3;
// Imaginary number
ImD = 2*%i;


RePM = [ReP; 2*ReP];
ImPM = [ImP; 2*ImP];
ReDM = [ReD; 5*ReD];
ImDM = [ImD; ImD/2];

// Real Poly .* Im Double
res = ReP .* ImD;
assert_checktrue(real(res) == 0);
assert_checkequal(imag(res), ReP * imag(ImD));

// Real Poly Matrix .* Im Double
res = RePM .* ImD;
assert_checktrue(real(res)(1) == 0);
assert_checktrue(real(res)(2) == 0);
assert_checkequal(imag(res)(1), RePM(1) * imag(ImD));
assert_checkequal(imag(res)(2), RePM(2) * imag(ImD));

// Real Poly .* Im Double Matrix
res = ReP .* ImDM;
assert_checktrue(real(res)(1) == 0);
assert_checktrue(real(res)(2) ==  0);
assert_checkequal(imag(res)(1), ReP * imag(ImDM(1)));
assert_checkequal(imag(res)(2), ReP * imag(ImDM(2)));

// Real Poly Matrix .* Im Double Matrix
res = RePM .* ImDM;
assert_checktrue(real(res)(1) == 0);
assert_checktrue(real(res)(2) == 0);
assert_checkequal(imag(res)(1), RePM(1) * imag(ImDM(1)));
assert_checkequal(imag(res)(1), RePM(2) * imag(ImDM(2)));

// Imaginary Poly * Imaginary number
res = ImP .* ImD;
assert_checkequal(real(res), -1 * imag(ImP) * imag(ImD));
assert_checktrue(imag(res) == 0);

// Im Poly Matrix .* Im Double
res = ImPM .* ImD;
assert_checkequal(real(res)(1), -1 * imag(ImPM)(1) * imag(ImD));
assert_checkequal(real(res)(2), -1 * imag(ImPM)(2) * imag(ImD));
assert_checktrue(imag(res)(1) == 0);
assert_checktrue(imag(res)(2) == 0);

// Im Poly .* Im Double Matrix
res = ImP .* ImDM;
assert_checkequal(real(res)(1), -1 * imag(ImP) * imag(ImDM)(1));
assert_checkequal(real(res)(2), -1 * imag(ImP) * imag(ImDM)(2));
assert_checktrue(imag(res)(1) == 0);
assert_checktrue(imag(res)(2) == 0);

// Im Poly Matrix .* Im Double Matrix
res = ImPM .* ImDM;
assert_checkequal(real(res)(1), -1 * imag(ImPM)(1) * imag(ImDM)(1));
assert_checkequal(real(res)(2), -1 * imag(ImPM)(2) * imag(ImDM)(2));
assert_checktrue(imag(res)(1) == 0);
assert_checktrue(imag(res)(2) == 0);

// Complex Poly Matrix .* Complex Double Matrix
CoPM = RePM + ImPM;
CoDM = ReDM + ImDM;

res = CoPM .* CoDM;
assert_checkequal(real(res)(1), real(CoPM)(1) * real(CoDM)(1) - imag(CoPM)(1) * imag(CoDM)(1));
assert_checkequal(real(res)(2), real(CoPM)(2) * real(CoDM)(2) - imag(CoPM)(2) * imag(CoDM)(2));
assert_checkequal(imag(res)(1), real(CoPM)(1) * imag(CoDM)(1) + imag(CoPM)(1) * real(CoDM)(1));
assert_checkequal(imag(res)(2), real(CoPM)(2) * imag(CoDM)(2) + imag(CoPM)(2) * real(CoDM)(2));

