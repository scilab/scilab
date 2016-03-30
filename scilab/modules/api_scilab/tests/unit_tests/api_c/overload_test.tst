// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);

function test_overload(safe)
    mkdir(pathconvert(TMPDIR+"/api_c/" + safe));
    cd(pathconvert(TMPDIR+"/api_c/" + safe));
    copyfile(SCI+"/modules/api_scilab/tests/unit_tests/api_c/overload_test.c",pathconvert(TMPDIR+"/api_c/" + safe + "/overload_test.c",%F));

    cflags = "";
    if safe == "unsafe" then
        cflags = "-D__API_SCILAB_UNSAFE__";
    end

    ilib_build("liboverload" + safe,["overload_test","sci_overload_test", "csci6"],"overload_test.c",[],"","",cflags);
    exec("loader.sce");

    a = rand(10, 10);
    x = overload_test("cos", a);
    assert_checkequal(x, cos(a));

    function x = %s_overload_test(a)
        x = cos(a);
    endfunction

    x = overload_test(a);
    assert_checkequal(x, cos(a));
    disp("OK");
endfunction

test_overload("safe");
test_overload("unsafe");
