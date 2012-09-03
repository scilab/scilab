// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/string_reading_api"));
cd(pathconvert(TMPDIR+"/string_reading_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/string_reading_api.c",pathconvert(TMPDIR+"/string_reading_api/string_reading_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("string_reading",["read_string","read_string"],"string_reading_api.c",[],"","",cflags);
exec("loader.sce");

a_ref = ["may the puffin be with you"];
a = ["may", "the", "puffin"; "be","with","you"];
b = read_string(a);
if a_ref <> b then pause;end
