// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);

function test_cell(safe)
    mkdir(pathconvert(TMPDIR+"/api_c/" + safe));
    cd(pathconvert(TMPDIR+"/api_c/" + safe));
    copyfile(SCI+"/modules/api_scilab/tests/unit_tests/api_c/cell_test.c",pathconvert(TMPDIR+"/api_c/" + safe + "/cell_test.c",%F));

    cflags = "";
    if safe == "unsafe" then
        cflags = "-D__API_SCILAB_UNSAFE__";
    end

    ilib_build("libcell" + safe,["cell_test","sci_cell_test", "csci6"],"cell_test.c",[],"","",cflags);
    exec("loader.sce");

    data = list(1, "2", %t, %s, list(1,2,3), {1,2;3,4});
    c = cell_test([2 3], data);

    assert_checkequal(typeof(c), "ce");
    assert_checkequal(c{1, 1}, 1);
    assert_checkequal(c{2, 1}, "2");
    assert_checkequal(c{1, 2}, %t);
    assert_checkequal(c{2, 2}, %s);
    assert_checkequal(c{1, 3}, list(1,2,3));
    assert_checkequal(c{2, 3}, {1,2;3,4});

    disp("OK");
endfunction

test_cell("safe");
test_cell("unsafe");
