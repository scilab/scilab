// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/doubleExample"));
cd(pathconvert(TMPDIR+"/doubleExample"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/doubleExample.c",pathconvert(TMPDIR+"/doubleExample/doubleExample.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("gw_doubleExample",["doubleExample","doubleExample"],"doubleExample.c",[],"","",cflags);
exec("loader.sce");

a = 1;
b = %i;
c = [1,2,3;4,5,6];
d = c(1:$) + c($:-1:1) * %i;
assert_checkequal(doubleExample(a), a);
assert_checkequal(doubleExample(b), b);
assert_checkequal(doubleExample(c), c);
assert_checkequal(doubleExample(d), d);
