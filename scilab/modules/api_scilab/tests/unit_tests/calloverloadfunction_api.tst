// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/calloverloadfunction_api"));
cd(pathconvert(TMPDIR+"/calloverloadfunction_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/calloverloadfunction_api.c",pathconvert(TMPDIR+"/calloverloadfunction_api/calloverloadfunction_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("gw_callOverload", ["getSize", "callOverload"], "calloverloadfunction_api.c", [], "", "", cflags);
exec("loader.sce");

function x = %c_getSize(a)
    x = length(a);
endfunction

function x = %s_getSize(a)
    x = size(a);
endfunction


a = [1,2;3,4];
a_ref = [2,2];
assert_checkequal(getSize(a), a_ref);

b = ["May","the","puffin";"be","with","you"];
b_ref = [3,3,6;2,4,3];
assert_checkequal(getSize(b), b_ref);
