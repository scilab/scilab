// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12566 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12566
//
// <-- Short Description -->
// disp() generated an error on a tlist having a single field with an encoded
// integer value

test = tlist(["test","field"]);
T = [1 2 4 8 11 12 14 18];
test.field = int32(1);
for t = T
    test.field = iconvert(1,t);
    disp(test)
end
