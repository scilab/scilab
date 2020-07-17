// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16454 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16454
//
// <-- Short Description -->
// Sorting a sparse vector including some NaN yielded an error

s = sprand(20,30,0.5);
s([30 100 150 437]) = %nan;
objects = list(s(:)', s(:), s);
for o = objects
    for m = ["g" "r" "c" "lr" "lc"]
        assert_checktrue(execstr("gsort(o,m,""i"")", "errcatch")==0);
    end
end
