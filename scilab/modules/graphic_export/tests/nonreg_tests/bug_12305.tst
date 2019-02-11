// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
// Copyright (C) 2013 - Test case - Luc GABRIEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 12305 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12305
//
// <-- Short Description -->
//  Legends could not be loaded from SOD file.
//  Test : both graphics must have legends.

// curve 1 => blue
// curve 2 => red
// curve 3 => green
plot([1,2], [1,10], "b");
plot([1,2], [1,20], "r");
plot([1,2], [1,30], "g");
labels = ["1", "2", "3"];
legend(labels);

// save and reload figure
a = gca();
save(fullfile(TMPDIR, "bug_12305.dat"), "a");
clear a;
f1 = scf(0);
f2 = scf(1);
load(fullfile(TMPDIR, "bug_12305.dat"), "a");

xs2bmp(f1, TMPDIR + "/original.bmp");
xs2bmp(f2, TMPDIR + "/restored.bmp");

assert_checkequal(getmd5(TMPDIR + "/original.bmp"), getmd5(TMPDIR + "/restored.bmp"));
