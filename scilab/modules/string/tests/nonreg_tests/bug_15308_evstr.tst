// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 15308 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15308
//
// <-- Short Description -->
//  evstr(["1;2" "3;4"]) was refused, while evstr(["1 2" ; "3 4"]) is accepted

assert_checkequal(execstr("evstr([""1;2"",""3;4""])", "errcatch"), 0);
assert_checkequal(evstr(["1;2","3;4"]), [1 3 ; 2 4]);

assert_checkequal(evstr(["1;2 // A", "3;4 // B //"]), [1 3 ; 2 4]);
assert_checkequal(evstr(["1;2 // A", "3;4 // B //"; "5// C" "6// D"]), [1 3 ; 2 4 ; 5 6]);
