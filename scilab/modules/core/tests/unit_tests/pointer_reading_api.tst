// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/pointer_reading_api"));
cd(pathconvert(TMPDIR+"/pointer_reading_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("pointer_reading",["read_pointer","read_pointer"],SCI+"/modules/core/tests/unit_tests/pointer_reading_api.c",[],[],"",cflags);
exec("loader.sce");

            
b_ref = [1,2;3,4];
a = read_pointer();
b = read_pointer(a);
if or(b <> b_ref) then pause;end
            
        
