// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/poly_writing_api"));
cd(pathconvert(TMPDIR+"/poly_writing_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/poly_writing_api.c",pathconvert(TMPDIR+"/poly_writing_api/poly_writing_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("poly_writing",["write_poly","write_poly"],"poly_writing_api.c",[],"","",cflags);
exec("loader.sce");

p_ref = [2 5 18 1 -4 0 0 1 -14 0 0 4 0 0 0 0 0 0;64 1 0 0 0 0 -12 0 0 2 0 32 0 0 0 0 0 8];
l = list();
a = write_poly();
p = coeff(a);
if or(p <> p_ref) then pause;end
