// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8646 -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8646
//
// <-- Short Description -->
// The datatips contextual menu opens a selection list which is not ergonomic

plot2d();
// Click on: Edit -> Start datatip manager
// Put some datatips on the curves, with left button
// Right click opens a contextual menu: click on each selection and
// check that behaves as expected
