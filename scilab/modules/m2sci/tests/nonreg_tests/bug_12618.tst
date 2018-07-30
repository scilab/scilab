// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12618 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12618
//
// <-- Short Description -->
// i_notation() failed when a comment is appended, as with
// "ab8i = 2i*r; s = ''now 3i as text''; % 4i in comment"

txt = "r=1; ab8i = 2i*r; s = ''now 3i as text''; % 4i in comment";
path = TMPDIR+"/bug_12618.m";
mputl(txt, path);
mfile2sci(path, TMPDIR);
res = mgetl(TMPDIR+"/bug_12618.sci");
assert_checkequal(res($), "r = 1;ab8i = (2*%i)*r;s = ""now 3i as text"";// 4i in comment");
