// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

function test_boolean(safe)
    ilib_verbose(0);
    mkdir(pathconvert(TMPDIR+"/api_c/" + safe));
    cd(pathconvert(TMPDIR+"/api_c/" + safe));
    copyfile(SCI+"/modules/api_scilab/tests/unit_tests/api_c/boolean_test.c",pathconvert(TMPDIR+"/api_c/" + safe + "/boolean_test.c",%F));

    cflags = "";
    if safe == "unsafe" then
        cflags = "-D__API_SCILAB_UNSAFE__";
    end

    ilib_build("libboolean" + safe,["boolean_test","sci_boolean_test", "csci6"],"boolean_test.c",[],"","",cflags);
    exec("loader.sce");

    in1 = rand(3,4) > 0.5;
    in2 = rand() > 0.5;

    [out1, out2, out3] = boolean_test(in1, in2);

    assert_checkequal(out1, ~in1);
    ref(:,:, 1) = in1;
    ref(:,:, 2) = out1;
    assert_checkequal(out2, ref);
    assert_checkequal(out3, ~in2);
    disp("OK");
endfunction

test_boolean("safe");
test_boolean("unsafe");
