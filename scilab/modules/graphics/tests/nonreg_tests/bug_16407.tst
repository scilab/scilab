// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16407 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16407
//
// <-- Short Description -->
// Fec rendering is incorrect

x=[0 1 0]';
y=[0 0 1]';
z=[0 1 1]';
triangles=[1 1 2 3 1];
clf
n=4;
gcf().color_map = jetcolormap(n)
fec(x,y,triangles,z);
// hide axes because different stacking between Windows and others
gca().axes_visible="off"
xs2png(0,fullfile(TMPDIR,"bug_16407_1.png"))
//
w = 0.125;
xfrect(0,w,w,w)
gce().background = 1;
xfrect(0.5-w,0.5,w,w)
gce().background = n;
xs2png(0,fullfile(TMPDIR,"bug_16407_2.png"))
//
res1 = getmd5(fullfile(TMPDIR,"bug_16407_1.png"));
res2 = getmd5(fullfile(TMPDIR,"bug_16407_2.png"));
assert_checkequal(res1,res2)

