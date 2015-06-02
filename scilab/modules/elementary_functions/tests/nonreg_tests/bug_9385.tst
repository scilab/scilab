// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9385 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9385
//
// <-- Short Description -->
//    The type checking in trigonometric functions has been added
// =============================================================================

assert_checkfalse(execstr("tanm(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"), "tanm", 1);
assert_checkerror("tanm(""x"")", refMsg);

assert_checkfalse(execstr("tanhm(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix expected.\n"), "tanhm", 1);
assert_checkerror("tanhm(""x"")", refMsg);

assert_checkfalse(execstr("tanh(""x"")"   ,"errcatch") == 0);
refMsg = [msprintf(_("Function not defined for given argument type(s),\n"));msprintf(_("  check arguments or define function %s for overloading.\n"), "%c_tanh")];
assert_checkerror("tanh(""x"")", refMsg);

assert_checkfalse(execstr("tand(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"tand",1);
assert_checkerror("tand(""x"")", refMsg);

assert_checkfalse(execstr("sind(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"sind",1);
assert_checkerror("sind(""x"")", refMsg);

assert_checkfalse(execstr("sinc(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix or hypermatrix expected.\n"),"sinc",1);
assert_checkerror("sinc(""x"")", refMsg);

assert_checkfalse(execstr("sech(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"sech",1);
assert_checkerror("sech(""x"")", refMsg);

assert_checkfalse(execstr("secd(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"secd",1);
assert_checkerror("secd(""x"")", refMsg);

assert_checkfalse(execstr("sec(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix or hypermatrix expected.\n"),"sec",1);
assert_checkerror("sec(""x"")", refMsg);

assert_checkfalse(execstr("csgn(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix or hypermatrix expected.\n"), "csgn", 1);
assert_checkerror("csgn(""x"")", refMsg);

assert_checkfalse(execstr("csch(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"csch", 1);
assert_checkerror("csch(""x"")", refMsg);

assert_checkfalse(execstr("cscd(""x"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "cscd", 1);
assert_checkerror("cscd(""x"")", refMsg);

assert_checkfalse(execstr("csc(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix or hypermatrix expected.\n"), "csc", 1);
assert_checkerror("csc(""x"")", refMsg);

assert_checkfalse(execstr("cotd(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "cotd", 1);
assert_checkerror("cotd(""x"")", refMsg);

assert_checkfalse(execstr("cosd(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"cosd",1);
assert_checkerror("cosd(""x"")", refMsg);

assert_checkfalse(execstr("atand(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"atand",1);
assert_checkerror("atand(""x"")", refMsg);

assert_checkfalse(execstr("atand(1, ""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"atand",2);
assert_checkerror("atand(1, ""x"")", refMsg);

assert_checkfalse(execstr("asind(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"asind",1);
assert_checkerror("asind(""x"")", refMsg);

assert_checkfalse(execstr("asech(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix expected.\n"),"asech", 1);
assert_checkerror("asech(""x"")", refMsg);

assert_checkfalse(execstr("asecd(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real sparse or full matrix expected.\n"),"asecd",1);
assert_checkerror("asecd(""x"")", refMsg);

assert_checkfalse(execstr("asec(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix expected.\n"),"asec",1);
assert_checkerror("asec(""x"")", refMsg);

assert_checkfalse(execstr("acsch(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix expected.\n"),"acsch",1);
assert_checkerror("acsch(""x"")", refMsg);

assert_checkfalse(execstr("acscd(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real sparse or full matrix expected.\n"),"acscd",1);
assert_checkerror("acscd(""x"")", refMsg);

assert_checkfalse(execstr("acsc(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix expected.\n"),"acsc",1);
assert_checkerror("acsc(""x"")", refMsg);

assert_checkfalse(execstr("acoth(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix expected.\n"),"acoth",1);
assert_checkerror("acoth(""x"")", refMsg);

assert_checkfalse(execstr("acotd(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"acotd",1);
assert_checkerror("acotd(""x"")", refMsg);

assert_checkfalse(execstr("acot(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"acot", 1);
assert_checkerror("acot(""x"")", refMsg);

assert_checkfalse(execstr("acosd(""x"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"acosd",1);
assert_checkerror("acosd(""x"")", refMsg);


