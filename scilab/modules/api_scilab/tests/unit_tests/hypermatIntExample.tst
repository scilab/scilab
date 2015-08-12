// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/hypermatIntExample"));
cd(pathconvert(TMPDIR+"/hypermatIntExample"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/hypermatIntExample.c",pathconvert(TMPDIR+"/hypermatIntExample/hypermatIntExample.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("gw_hypermatIntExample",["hypermatIntExample","hypermatIntExample"],"hypermatIntExample.c",[],"","",cflags);
exec("loader.sce");

a = hypermat([1 2 3], int8(1:6));
b = hypermat([1 2 3], uint8(1:6));
c = hypermat([1 2 3], int16(1:6));
d = hypermat([1 2 3], uint16(1:6));
e = hypermat([1 2 3], int32(1:6));
f = hypermat([1 2 3], uint32(1:6));

assert_checkequal(hypermatIntExample(a), a);
assert_checkequal(hypermatIntExample(b), b);
assert_checkequal(hypermatIntExample(c), c);
assert_checkequal(hypermatIntExample(d), d);
assert_checkequal(hypermatIntExample(e), e);
assert_checkequal(hypermatIntExample(f), f);
