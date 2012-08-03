// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
// <-- Non-regression test for bug 10365 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10365
//
// <-- Short Description -->
//    copy crashs on entity, target must be an axe

txt = ["${Scilab}$" "${is}$";"${beautiful}$", "${:)}$"];
xstring(0.5, 0.5, txt)
e1=gce();
f2 = scf();
a2 = gca();
err = execstr("copy(e1, f2)", "errcatch");
assert_checktrue(err <> 0);
err = execstr("copy(e1, a2)", "errcatch");
assert_checktrue(err == 0);
