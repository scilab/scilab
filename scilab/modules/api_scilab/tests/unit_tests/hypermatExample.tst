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
ilib_build("gw_hypermatExample",["hypermatExample","hypermatExample"],"hypermatExample.c",[],"","","");
exec("loader.sce");

a = matrix(ones(1,6), [1 2 3]);
b = a + %i*matrix(1:6, [1 2 3]);

assert_checkequal(hypermatExample(a), a);
assert_checkequal(hypermatExample(b), b);
