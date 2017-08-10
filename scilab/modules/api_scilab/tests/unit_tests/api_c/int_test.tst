// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
ilib_verbose(0);

function test_int(safe)
    mkdir(pathconvert(TMPDIR+"/api_c/" + safe));
    cd(pathconvert(TMPDIR+"/api_c/" + safe));
    copyfile(SCI+"/modules/api_scilab/tests/unit_tests/api_c/int_test.c",pathconvert(TMPDIR+"/api_c/" + safe + "/int_test.c",%F));

    cflags = "";
    if safe == "unsafe" then
        cflags = "-D__API_SCILAB_UNSAFE__";
    end
    
    ilib_build("libint" + safe,["int_test","sci_int_test", "csci6"],"int_test.c",[],"","",cflags);
    exec("loader.sce");
    
    in1 = int32(rand(3,4) *1000);
    in2 = int32(rand() * 1000);

    [out1, out2, out3] = int_test(in1, in2);

    assert_checkequal(out1, in1 * 10);
    ref(:,:, 1) = in1 * 10;
    ref(:,:, 2) = in1 * 100;
    assert_checkequal(out2, ref);
    assert_checkequal(out3, in2 * 1000);
    disp("OK");
endfunction

test_int("safe");
test_int("unsafe");
