// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for nancumsum function 
// =============================================================================

assert_checkfalse(execstr("nancumsum()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "nancumsum", 1, 2);
assert_checkerror("nancumsum()", refMsg);

assert_checkfalse(execstr("nancumsum(""s"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, boolean, polynomial matrix expected.\n"), "nancumsum", 1);
assert_checkerror("nancumsum(""s"")", refMsg);

assert_checkfalse(execstr("nancumsum(1, ""p"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "nancumsum", 2, """r"", ""c"", ""*"", 1, 2");
assert_checkerror("nancumsum(1, ""p"")", refMsg);

assert_checkfalse(execstr("nancumsum(1, %s)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string or scalar expected.\n"), "nancumsum", 2);
assert_checkerror("nancumsum(1, %s)", refMsg);

assert_checkequal(nancumsum([]), []);
assert_checkequal(nancumsum(%nan), []);
assert_checkequal(nancumsum([%nan %nan; 1 1]), [0 1; 1 2]);
assert_checkequal(nancumsum([%nan %nan; 1 1], "*"), [0 1; 1 2]);
assert_checkequal(nancumsum([%nan %nan; 1 1], "r"), [0 0; 1 1]);
assert_checkequal(nancumsum([%nan %nan; 1 1], "c"), [0 0; 1 2]);
assert_checkequal(nancumsum([%nan %nan; 1 1], 1), [0 0; 1 1]);
assert_checkequal(nancumsum([%nan %nan; 1 1], 2), [0 0; 1 2]);

assert_checkequal(nancumsum([%nan 1; %nan 1]), [0 1;0 2]);
assert_checkequal(nancumsum([%nan 1; %nan 1], "*"), [0 1;0 2]);
assert_checkequal(nancumsum([%nan 1; %nan 1], "r"), [0 1;0 2]);
assert_checkequal(nancumsum([%nan 1; %nan 1], "c"), [0 1;0 1]);
assert_checkequal(nancumsum([%nan 1; %nan 1], 1), [0 1;0 2]);
assert_checkequal(nancumsum([%nan 1; %nan 1], 2), [0 1;0 1]);

assert_checkequal(nancumsum([1 3; %nan 6]), [1 4; 1 10]);
assert_checkequal(nancumsum([1 3; %nan 6], "*"), [1 4; 1 10]);
assert_checkequal(nancumsum([1 3; %nan 6], "r"), [1 3; 1 9]);
assert_checkequal(nancumsum([1 3; %nan 6], "c"), [1 4; 0 6]);
assert_checkequal(nancumsum([1 3; %nan 6], 1), [1 3; 1 9]);
assert_checkequal(nancumsum([1 3; %nan 6], 2), [1 4; 0 6]);

assert_checkequal(nancumsum([%nan %s; 1 1]), [0 1+%s; 1 2+%s]);
assert_checkequal(nancumsum([%nan %s; 1 1], "*"), [0 1+%s; 1 2+%s]);
assert_checkequal(nancumsum([%nan %s; 1 1], "r"), [0 %s; 1 1+%s]);
assert_checkequal(nancumsum([%nan %s; 1 1], "c"), [0 %s; 1 2]);
assert_checkequal(nancumsum([%nan %s; 1 1], 1), [0 %s; 1 1+%s]);
assert_checkequal(nancumsum([%nan %s; 1 1], 2), [0 %s; 1 2]);

assert_checkequal(nancumsum([%nan %t]), [0 1]);
assert_checkequal(nancumsum([%nan %t], "*"), [0 1]);
assert_checkequal(nancumsum([%nan %t], "r"), [0 1]);
assert_checkequal(nancumsum([%nan %t], "c"), [0 1]);
assert_checkequal(nancumsum([%nan %t], 1), [0 1]);
assert_checkequal(nancumsum([%nan %t], 2), [0 1]);
