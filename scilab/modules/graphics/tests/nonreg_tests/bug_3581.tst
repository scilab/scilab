// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3581 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3581
//
// <-- Short Description -->
// Legends do not appear correctly when axes are plotted in log scale.
// 

f=scf(0);
clf;
plot2d([0.01:0.01:0.1 0.2:0.1:1 2:1:10],[0.01:0.01:0.1 0.2:0.1:1 2:1:10].^2,style=1);
plot2d([0.01:0.01:0.1 0.2:0.1:1 2:1:10],[0.01:0.01:0.1 0.2:0.1:1 2:1:10].^2.5,style=2);
plot2d([0.01:0.01:0.1 0.2:0.1:1 2:1:10],[0.01:0.01:0.1 0.2:0.1:1 2:1:10],style=3);
plot2d([0.01:0.01:0.1 0.2:0.1:1 2:1:10],sqrt([0.01:0.01:0.1 0.2:0.1:1 2:1:10]),style=4);
legend('one','two','three','four',2);
f.children.children.children(2:5).mark_mode="on";
f.children.children.children(2:5).mark_style=4;
f.children.children.children(2:5).mark_size_unit="point";
f.children.children.children(2:5).mark_size=4;
f.children.log_flags='ll';

// check that the legend line samples and text are facing.