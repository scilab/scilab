// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/sparseExample"));
cd(pathconvert(TMPDIR+"/sparseExample"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/sparseExample.c",pathconvert(TMPDIR+"/sparseExample/sparseExample.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("sparseExample",["sparseExample","sparseExample"],"sparseExample.c",[],"","",cflags);
exec("loader.sce");

a = sparse([1,1;2,2;3,3], [1, 2, 3]);
b = sparse([1,1;2,2;3,3], [1 + 2 * %i, 3 , -4 * %i]);
if sparseExample(a) <> a then pause;end
if sparseExample(b) <> b then pause;end
