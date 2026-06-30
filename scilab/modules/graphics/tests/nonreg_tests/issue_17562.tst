// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->
// Ref is needed to check the warning message is no more displayed

// <-- Non-regression test for bug 17562 -->
//
// <-- Bugzilla URL -->
// https://gitlab.com/scilab/scilab/-/issues/17562
//
// <-- Short Description -->
// Some values no more work as "Marker" property when calling plot() since Scilab 2024.0.0

plot(1:10, "marker", ">");
plot(1:10, "marker", "<");
plot(1:10, "marker", "v");
plot(1:10, "marker", "^");

