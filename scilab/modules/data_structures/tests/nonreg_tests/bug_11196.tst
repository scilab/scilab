// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 11196 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11196
//
// <-- Short Description -->
// Some recursive extractions from a mlist could fail.
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

A = mlist('A');
deff('y = %A_e(field, ml)','y = mlist(''A'')');

// Should still succeed:
assert_checkequal(execstr("A.a.b.c.d.e", "errcatch"), 0);
assert_checkequal(execstr("A(''a'').b.c.d.e", "errcatch"), 0);
assert_checkequal(execstr("A.a.b(''c'')(''d'').e", "errcatch"), 0);
assert_checkequal(execstr("A.a()", "errcatch"), 0);

// Should now succeed:
assert_checkequal(execstr("A().a", "errcatch"), 0);
assert_checkequal(execstr("A.a.b()", "errcatch"), 0);
