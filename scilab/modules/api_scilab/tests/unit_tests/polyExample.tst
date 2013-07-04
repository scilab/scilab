// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/polyExample"));
cd(pathconvert(TMPDIR+"/polyExample"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/polyExample.c",pathconvert(TMPDIR+"/polyExample/polyExample.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("gw_polyExample",["polyExample","polyExample"],"polyExample.c",[],"","",cflags);
exec("loader.sce");

a = %s;
b = (2 + 3*%i) * %s;
c = [a, 2*a, 3*a;4*a, 5*a, 6*a];
d = [c + c * %i];
assert_checkequal(polyExample(a), a);
assert_checkequal(polyExample(b), b);
assert_checkequal(polyExample(c), c);
assert_checkequal(polyExample(d), d);
