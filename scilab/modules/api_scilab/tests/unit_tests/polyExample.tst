// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/polyExample"));
cd(pathconvert(TMPDIR+"/polyExample"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/polyExample.c",pathconvert(TMPDIR+"/polyExample/polyExample.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("polyExample",["polyExample","polyExample"],"polyExample.c",[],"","",cflags);
exec("loader.sce");

a = %s;
b = (2 + 3*%i) * %s;
c = [a, 2*a, 3*a;4*a, 5*a, 6*a];
d = [c + c * %i];
if polyExample(a) <> a then pause, end
if polyExample(b) <> b then pause, end
if polyExample(c) <> c then pause, end
if polyExample(d) <> d then pause, end
