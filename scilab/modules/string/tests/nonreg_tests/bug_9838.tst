// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 9838 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9838
//
// <-- Short Description -->
// evstr() froze on text components having some comments but no result
//
ni = "// no instruction";
assert_checkequal(evstr(ni),[]);
assert_checkequal(evstr([ni ni]),[]);
assert_checkequal(evstr([ni ni]'),[]);
t = [ni, " "+ni, "[] "+ni, "[] "+ni, " [] "+ni];
assert_checkequal(evstr(t), []);
t = [ t "3"];
assert_checkequal(evstr(t),  [%nan %nan %nan %nan %nan 3]);
assert_checkequal(evstr(t'), [%nan %nan %nan %nan %nan 3]');
t($) = "%t";
assert_checkequal(evstr(t),[%f %f %f %f %f %t]);
t($) = """AbCd""";
assert_checkequal(evstr(t),["" "" "" "" "" "AbCd"]);
t($) = "int8(37)";
assert_checkequal(evstr(t),int8([0 0 0 0 0 37]));

