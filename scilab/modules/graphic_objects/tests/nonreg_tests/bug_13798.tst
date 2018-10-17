// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 13798 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13798
//
// <-- Short Description -->
// Datatips does not update when moving a curve or changing its data
plot([0 1],[0 1]);
e = gce().children;
d = datatipCreate(e,[.5,.5]);
e.data(1,:) = [0.1,0.5];
assert_checkequal(d.text,["X:0.55";"Y:0.75"]);
