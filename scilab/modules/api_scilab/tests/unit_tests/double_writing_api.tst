// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/double_writing_api"));
cd(pathconvert(TMPDIR+"/double_writing_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/double_writing_api.c",pathconvert(TMPDIR+"/double_writing_api/double_writing_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("double_writing",["write_double","write_double"],"double_writing_api.c",[],"","",cflags);
exec("loader.sce");

a_ref = [   0 1 2 3; ..
            4 5 6 7; ..
            8 9 10 11];
b_ref = [   23*%i,      1+22*%i,    2+21*%i,    3+20*%i,    4+19*%i,    5+18*%i; ..
            6+17*%i,    7+16*%i,    8+15*%i,    9+14*%i,     10+13*%i,  11+12*%i; ..
            12+11*%i,   13+10*%i,   14+9*%i,    15+8*%i,    16+7*%i,    17+6*%i; ..
            18+5*%i,    19+4*%i,    20+3*%i,    21+2*%i,    22+1*%i,    23];
[a,b] = write_double();
if or(a <> a_ref) then pause;end
if or(b <> b_ref) then pause;end
