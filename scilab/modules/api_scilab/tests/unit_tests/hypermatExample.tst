// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/hypermatExample"));
cd(pathconvert(TMPDIR+"/hypermatExample"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/hypermatExample.c",pathconvert(TMPDIR+"/hypermatExample/hypermatExample.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("gw_hypermatExample",["hypermatExample","hypermatExample"],"hypermatExample.c",[],"","",cflags);
exec("loader.sce");

a = hypermat([1 2 3], ones(1,6));
b = a + %i*hypermat([1 2 3], 1:6);

assert_checkequal(hypermatExample(a), a);
assert_checkequal(hypermatExample(b), b);
