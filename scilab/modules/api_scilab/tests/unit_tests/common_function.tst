// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/common_function"));
cd(pathconvert(TMPDIR+"/common_function"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/common_function_api.c",pathconvert(TMPDIR+"/common_function/common_function_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("gw_common_function",["common_function","common_function"],"common_function_api.c",[],"","",cflags);
exec("loader.sce");

l1 = [1,2*%i,3;%i,2,3*%i];
l2 = ["may","the";"puffin","be";"with","you"];
l3 = int8([1,2,3]);
l4 = uint16([1000,2000,3000]);
l5 = list(l1,l2,l3);
l = list(l1,l2,l3,l4,l5);
common_function(l(1:$))
