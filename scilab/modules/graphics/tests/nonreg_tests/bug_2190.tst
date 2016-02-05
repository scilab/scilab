// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 2190 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2190
//
// <-- Short Description -->
// Scilab crashes when trying to plot2d huge amounts of data.

p=1:9000000;
q=9000000:-1:1;
plot2d(p,q)

// scilab should display an error message, but not crash.

