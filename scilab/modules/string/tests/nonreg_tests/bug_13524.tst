// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 13524 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13524
//
// <-- Short Description -->
// strtod did not ignore the tabs and CR
//

// <-- CLI SHELL MODE -->

// check \t
str = msprintf("\t 10");
v = strtod(str);
assert_checkequal(v, 10);
str = msprintf("\t 1.5");
v = strtod(str);
assert_checkequal(v, 1.5);
str = msprintf("\t          1.5");
v = strtod(str);
assert_checkequal(v, 1.5);
str = msprintf("        \t          1.5");
v = strtod(str);
assert_checkequal(v, 1.5);
str = msprintf("\n  \t  1.5");
v = strtod(str);
assert_checkequal(v, [%nan; 1.5]);
str = msprintf("\t  1.5end");
[v, endStr] = strtod(str);
assert_checkequal(v, 1.5);
assert_checkequal(endStr, "end");
// check \r
str = msprintf("\r 10");
v = strtod(str);
assert_checkequal(v, 10);
str = msprintf("\r 1.5");
v = strtod(str);
assert_checkequal(v, 1.5);
str = msprintf("\r          1.5");
v = strtod(str);
assert_checkequal(v, 1.5);
str = msprintf("        \r          1.5");
v = strtod(str);
assert_checkequal(v, 1.5);
str = msprintf("\n  \r  1.5");
v = strtod(str);
assert_checkequal(v, [%nan; 1.5]);
str = msprintf("\r  1.5end");
[v, endStr] = strtod(str);
assert_checkequal(v, 1.5);
assert_checkequal(endStr, "end");
