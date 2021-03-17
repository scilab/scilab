// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16357 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16357
//
// <-- Short Description -->
// script2var failed found variables defined by an exec() call
//

[st, ierr] = script2var("", struct());
assert_checkequal(st, struct());

[st, ierr] = script2var("a=1", struct());
assert_checkequal(st.a, 1);

[st, ierr] = script2var("a=1;b=2", struct());
assert_checkequal(st.a, 1);
assert_checkequal(st.b, 2);

f=fullfile(TMPDIR, "vars.ini");
mputl("a=1", f);

[st, ierr] = script2var("exec(f, -1);b=2", struct());
assert_checkequal(st.a, 1);
assert_checkequal(st.b, 2);

[st, ierr] = script2var("a=1; clear;b=2", struct());
assert_checkequal(st.b, 2);
assert_checkequal(fieldnames(st), "b");
