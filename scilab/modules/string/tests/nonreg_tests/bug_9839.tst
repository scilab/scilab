// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 9839 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9839
//
// <-- Short Description -->
// evstr() failed when some components include ascii(10) or/and ascii(13) 
//         linebreak characters

a10 = ascii(10);
assert_checkequal(evstr(a10+"4"), []);
assert_checkequal(evstr([a10+"4" "5"]), [%nan 5]);
assert_checkequal(evstr([a10+"4" ; "5"]), [%nan ; 5]);
assert_checkequal(evstr(["3"+a10+"4" "5"]), [3 5]);
assert_checkequal(evstr(["3"+a10+"4" ; "5"]), [3 ; 5]);
assert_checkequal(evstr(["%i"+a10+"4" "5"]), [%i 5]);
assert_checkequal(evstr(["%i"+a10+"4" ; "5"]), [%i ; 5]);

a13 = ascii(13);
assert_checkequal(evstr(a13+"4"), []);
assert_checkequal(evstr([a13+"4" "5"]), [%nan 5]);
assert_checkequal(evstr([a13+"4" ; "5"]), [%nan ; 5]);
assert_checkequal(evstr(["3"+a13+"4" "5"]), [3 5]);
assert_checkequal(evstr(["3"+a13+"4" ; "5"]), [3 ; 5]);
assert_checkequal(evstr(["%i"+a13+"4" "5"]), [%i 5]);
assert_checkequal(evstr(["%i"+a13+"4" ; "5"]), [%i ; 5]);
