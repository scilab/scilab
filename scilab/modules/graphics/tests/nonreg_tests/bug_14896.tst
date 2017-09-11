// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14896 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14896
//
// <-- Short Description -->
// Using 'set' with multiple properties, only first one is set.

u = uicontrol();
set(u, "position", [10 10 100 100], "string", "ABCDE", "relief", "solid");
assert_checkequal(u.string, "ABCDE");
assert_checkequal(u.relief, "solid");
