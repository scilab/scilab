// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHICS -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16734 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16734
//
// <-- Short Description -->
// findobj severely degrades run time performance in Scilab 6.1.1

clf
plot();
gce().children(7).tag = "foo";

N=1000;
tic();
for i=1:N
    h = findobj("tag","foo");
end
t = toc()/N;

assert_checktrue(t<1e-4)
