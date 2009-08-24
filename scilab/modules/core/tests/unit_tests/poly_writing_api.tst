// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/poly_writing_api"));
cd(pathconvert(TMPDIR+"/poly_writing_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("poly_writing",["write_poly","write_poly"],SCI+"/modules/core/tests/unit_tests/poly_writing_api.c",[],[],"",cflags);
exec("loader.sce");

            
p_ref = [2 5 18 1 -4 0 0 1 -14 0 0 4 0 0 0 0 0 0;64 1 0 0 0 0 -12 0 0 2 0 32 0 0 0 0 0 8];
l = list();
a = write_poly();
p = coeff(a);
if or(p <> p_ref) then pause;end
            
        
