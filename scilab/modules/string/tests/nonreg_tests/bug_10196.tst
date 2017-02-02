// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 10196 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10196
//
// <-- Short Description -->
// evstr(s):
//  a) when s is scalar and its result is void, an error occured
//  b) when s is a vector and one of its components is void or empty, this one
//     was not returned and next components were shifted.
//  c) when s is a matrix and one of its components is void or empty,
//     an error occured

assert_checkequal(execstr("evstr(""rand()"")", "errcatch"),0); // normal
assert_checkequal(evstr("1 2"), [1 2]);                        // normal
assert_checkequal(evstr("1;2"), [1; 2]);                       // normal
assert_checktrue(execstr("evstr(''whereami()'')", "errcatch") == 0); // debugged
assert_checkequal(evstr("whereami()"), []);     // debugged

//
assert_checkequal(evstr(["%pi" "[]"   "%i" "%eps"]), [%pi %nan   %i %eps]);
assert_checkequal(evstr(["%pi" "[]" ; "%i" "%eps"]), [%pi %nan ; %i %eps]);
assert_checkequal(evstr(["%t" "[]"   "%f" "%T"]), [%t %f   %f %t]);
assert_checkequal(evstr(["%t" "[]" ; "%f" "%T"]), [%t %f ; %f %t]);
assert_checkequal(evstr(["""A""" "[]"   """B""" """C"""]), ["A" ""   "B" "C"]);
assert_checkequal(evstr(["""A""" "[]" ; """B""" """C"""]), ["A" "" ; "B" "C"]);
t = ["int8(3)" "[]" ; "int8(4)" "int8(5)"];
assert_checkequal(evstr(t(:)'), int8([3 4 0 5]));
assert_checkequal(evstr(t),    int8([3 0 ; 4 5]));

assert_checkequal(evstr(["%pi" ""   "%i" "%eps"]), [%pi %nan   %i %eps]);
assert_checkequal(evstr(["%pi" "" ; "%i" "%eps"]), [%pi %nan ; %i %eps]);
assert_checkequal(evstr(["%t"  ""    "%f" "%T"]),  [%t %f   %f %t]);
assert_checkequal(evstr(["%t"  ""   ; "%f" "%T"]), [%t %f ; %f %t]);
assert_checkequal(evstr(["""A""" ""   """B""" """C"""]), ["A" ""   "B" "C"]);
assert_checkequal(evstr(["""A""" "" ; """B""" """C"""]), ["A" "" ; "B" "C"]);
t = ["int8(3)" "" ; "int8(4)" "int8(5)"];
assert_checkequal(evstr(t(:)'), int8([3 4 0 5]));
assert_checkequal(evstr(t),    int8([3 0 ; 4 5]));

assert_checkequal(evstr(["%pi" " // nothing"   "%i" "%eps"]), [%pi %nan   %i %eps]);
assert_checkequal(evstr(["%pi" " // nothing" ; "%i" "%eps"]), [%pi %nan ; %i %eps]);
assert_checkequal(evstr(["%t"  " // nothing"    "%f" "%T"]),  [%t %f   %f %t]);
assert_checkequal(evstr(["%t"  " // nothing"   ; "%f" "%T"]), [%t %f ; %f %t]);
assert_checkequal(evstr(["""A""" " // nothing"   """B""" """C"""]), ["A" ""   "B" "C"]);
assert_checkequal(evstr(["""A""" " // nothing" ; """B""" """C"""]), ["A" "" ; "B" "C"]);
t = ["int8(3)" " // nothing" ; "int8(4)" "int8(5)"];
assert_checkequal(evstr(t(:)'), int8([3 4 0 5]));
assert_checkequal(evstr(t),    int8([3 0 ; 4 5]));
