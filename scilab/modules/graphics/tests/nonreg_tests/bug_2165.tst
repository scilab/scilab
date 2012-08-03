// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 2165 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2165
//
// <-- Short Description -->
// Impossible to save Scilab variables when there are no more existing handles.

f = gcf();

delete(f);

warning("off");
if execstr("save(TMPDIR+""/bug_2165.tst"");", "errcatch")<>0 then pause; end
warning("on");
