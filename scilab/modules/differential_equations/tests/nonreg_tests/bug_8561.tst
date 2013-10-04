// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 8561 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8561
//
// <-- Short Description -->
//    Absolute and relative tolerances sizes were not properly checked.

m = 1; l = 1; g = 10;
theta0 = 0.2; fi0 = %pi/4;
x0 = l*sin(fi0)*cos(theta0);
y0 = l*cos(fi0)*cos(theta0);
z0 = l*sin(theta0);
Y0  = [x0; y0; z0; cos(fi0)/sin(fi0); -1; 0; 0];
Yd0 = [Y0(4:6) ; 0; 0; -g; 0];
t = 0:0.01:15;
function [res, ires] = f(t, y, yd)
  res = [yd(1:3) - y(4:6);
         yd(4:6) + 2*y(1:3)*y(7)/m + [0; 0; g];
         2*y(4:6)'*y(1:3)];
  ires = 0;
endfunction

// DASSL

//rtol and atol should have 7 elements, because neq = 7
rtol = [1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4];
atol = 2*rtol;

assert_checkfalse(execstr("y = dassl([Y0 Yd0], 0, t, atol, rtol, f);", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar or a matrix of size %d expected.\n"), "dassl", 4, 7);
assert_checkerror("y = dassl([Y0 Yd0], 0, t, atol, rtol, f);", refMsg);

rtol = [1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4, 1e-3];
atol = 2*rtol;
y = dassl([Y0 Yd0], 0, t, atol, rtol, f);
expected = [ 15.;
              0.1104081129188701354549;
              0.5384584912402592404845;
             -0.8336030864039580823288;
              1.7825915530763680738602;
             -3.8054019000300813857507;
             -2.2219595142170018853278;
             15.511893653170394813401;
              1.78229669499488441176;
             -3.8049312150854541769718;
             -2.2214274190024529964660;
             -3.4259257364524438038700;
            -16.6998600484128587595;
             15.860513004937361714042;
             33.034886001109668995923 ];
assert_checkalmostequal(y(:,1501), expected, 5*10^-1);

// DASRT

//rtol and atol should have 7 elements, because neq = 7
rtol = [1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4];
atol = 2*rtol;

assert_checkfalse(execstr("[y, n] = dasrt([Y0 Yd0], 0, t, atol, rtol, f, 0, ''gr1'');", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar or a matrix of size %d expected.\n"), "dasrt", 4, 7);
assert_checkerror("[y, n] = dasrt([Y0 Yd0], 0, t, atol, rtol, f, 0, ''gr1'');", refMsg);

rtol = [1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4, 1e-3];
atol = 2*rtol;
[y, n] = dasrt([Y0 Yd0], 0, t, atol, rtol, f, 0, 'gr1');
expected = [ 15.;
              0.1104081129188701354549;
              0.5384584912402592404845;
             -0.8336030864039580823288;
              1.7825915530763680738602;
             -3.8054019000300813857507;
             -2.2219595142170018853278;
             15.511893653170394813401;
              1.78229669499488441176;
             -3.8049312150854541769718;
             -2.2214274190024529964660;
             -3.4259257364524438038700;
            -16.6998600484128587595;
             15.860513004937361714042;
             33.034886001109668995923 ];
assert_checkalmostequal(y(:,1501), expected, 5*10^-1);

// DASKR

//rtol and atol should have 7 elements, because neq = 7
rtol = [1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4];
atol = 2*rtol;

assert_checkfalse(execstr("[y, n] = daskr([Y0 Yd0], 0, t, atol, rtol, f, 0, ''gr1'');", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar or a matrix of size %d expected.\n"), "daskr", 4, 7);
assert_checkerror("[y, n] = daskr([Y0 Yd0], 0, t, atol, rtol, f, 0, ''gr1'');", refMsg);

rtol = [1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4, 1e-3];
atol = 2*rtol;
[y, n] = daskr([Y0 Yd0], 0, t, atol, rtol, f, 0, 'gr1');
expected = [ 15.;
              0.1092127719669352264864;
              0.5544355723986162942651;
             -0.824314447102621028485;
              1.754185750256217479759;
             -3.7591067613357669330298;
             -2.2959555478405855311053;
             15.383436140631456368055;
              1.7541507611964979318486;
             -3.7586982499985461814163;
             -2.2961083123431151875593;
             -3.3610655360531245250399;
            -17.04596009480534490876;
             15.354244029596573639651;
             34.3716099776587071801 ];
assert_checkalmostequal(y(:,1501), expected, 5*10^-1);
