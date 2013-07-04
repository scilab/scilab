// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/poly_reading_api"));
cd(pathconvert(TMPDIR+"/poly_reading_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/poly_reading_api.c",pathconvert(TMPDIR+"/poly_reading_api/poly_reading_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("poly_reading",["read_poly","read_poly"],"poly_reading_api.c",[],"","",cflags);
exec("loader.sce");

coeff1 = [ ..
29*%i,22*%i,16*%i,11*%i,7*%i,30,23,17,12,8,-31*%i,-24*%i,-18*%i,-13*%i,-9*%i,32,25,19,14,10,-33*%i,-26*%i,-20*%i,-15*%i,0,34,27,21,0,0,0,-28*%i,0,0,0,36-35*%i,0,0,0,0; ..
4*%i,2*%i,%i,22,16,5,-3,0,-23*%i,-17*%i,-6*%i,0,0,24,18,0,0,0,-25*%i,-19*%i,0,0,0,26,20,0,0,0,-27*%i,-21*%i,0,0,0,28,0,0,0,0,0,0; ..
11,7,4,2,1,-12*%i,-8*%i,-5*%i,3*%i,0,13,9,6,0,0,-14*%i,-10*%i,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
x = poly(0, "x");
p1  = 1;
p2  = 2 * x + 3 * %i;
p3  = 4 * x**2 - 5 * %i * x + 6;
p4  = 7 * x**3 - 8 * %i * x**2 + 9 * x - 10 * %i;
p5  = 11 * x**4 - 12 * %i * x**3 + 13 * x**2 - 14 * %i * x + 15;
p6  = 16 * x**5 - 17 * %i * x**4 + 18 * x**3 - 19 * %i * x**2 + 20 * x  - 21 * %i;
p7  = 22 * x**6 - 23 * %i * x**5 + 24 * x**4 - 25 * %i * x**3 + 26 * x**2 - 27 * %i * x + 28;
p8  = %i;
p9  = 2 * %i * x - 3;
p10 = 4 * %i * x**2 + 5 * x - 6 * %i;
p11 = 7 * %i * x**3 + 8 * x**2 - 9 * %i * x + 10;
p12 = 11 * %i * x**4 + 12 * x**3 - 13 * %i * x**2 + 14 * x - 15 * %i;
p13 = 16 * %i * x**5 + 17 * x**4 - 18 * %i * x**3 + 19 * x**2 - 20 * %i * x  + 21;
p14 = 22 * %i * x**6 + 23 * x**5 - 24 * %i * x**4 + 25 * x**3 - 26 * %i * x**2 + 27 * x - 28 * %i;
p15 = 29 * %i * x**7 + 30 * x**6 - 31 * %i * x**5 + 32 * x**4 - 33 * %i * x**3 + 34 * x**2 - 35 * %i + 36;
p = [p1, p2, p3, p4, p5 ; p6, p7, p8, p9 ,p10 ; p11, p12, p13, p14, p15];
p1 = read_poly(p);
coeff2 = coeff(p1);
if or(coeff2 <> coeff1) then pause;end
