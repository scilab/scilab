// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 11472 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11472
//
// <-- Short Description -->
// sdf() changes figures menubar

S = get(0);
S.ShowHiddenHandles = "on";

f = gcf();
Nbrchild = length(f.children);
close(f);

sdf();
f = gcf();
assert_checkequal(length(f.children), Nbrchild);
close(f);
