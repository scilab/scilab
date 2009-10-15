// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4816 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4816
//
// <-- Short Description -->
// plot doesn't manage autoclear correctly.


a=gca();
a.auto_clear='on';
plot(1:10, 1:10, 'ro', 1:10, 10 - (1:10), 'b.'); // will it bug ?

