// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/string_reading_api"));
cd(pathconvert(TMPDIR+"/string_reading_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("string_reading",["read_string","read_string"],SCI+"/modules/core/tests/unit_tests/string_reading_api.c",[],[],"",cflags);
exec("loader.sce");

            
a_ref = ["may the puffin be with you"];
a_ref = ["sample strings manipulation with gateway API"];
a = ["may", "the", "puffin"; "be","with","you"];
b = read_string(a);
if a_ref <> b then pause;end
            
        
