// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

function test_polynomial(safe)
    ilib_verbose(0);
    mkdir(pathconvert(TMPDIR+"/api_c/" + safe));
    cd(pathconvert(TMPDIR+"/api_c/" + safe));
    copyfile(SCI+"/modules/api_scilab/tests/unit_tests/api_c/polynomial_test.c",pathconvert(TMPDIR+"/api_c/" + safe + "/polynomial_test.c",%F));

    cflags = "";
    if safe == "unsafe" then
        cflags = "-D__API_SCILAB_UNSAFE__";
    end

    ilib_build("libpolynomial" + safe,["polynomial_test","sci_polynomial_test", "csci6"],"polynomial_test.c",[],"","",cflags);
    exec("loader.sce");

    in1 = [10, 4*%s + 8, -5 + 6*%s - 7*%s**2;10, 4*%s + 8, -5 + 6*%s - 7*%s**2],
    in2 = 1-%s**2;

    [out1, out2] = polynomial_test(in1, in2);
    assert_checkequal(out1, in1 *%s);
    assert_checkequal(out2, -in2);
    disp("OK");
endfunction

test_polynomial("safe");
test_polynomial("unsafe");
