// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15995 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15995
//
// <-- Short Description -->
// Missing patch in surface plot (regression)

clf
surf(0:1,0:1,[0 1;1 %eps])
gcf().axes_size=[100 100]
xs2gif(0,"TMPDIR/image1.gif")

clf
surf(0:1,0:1,[0 1;1 0])
gcf().axes_size=[100 100]
xs2gif(0,"TMPDIR/image2.gif")

// bitmap images should be bitwise equal (patch has to be filled)
assert_checkequal(getmd5("TMPDIR/image1.gif"),getmd5("TMPDIR/image2.gif"))
