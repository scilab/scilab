// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - ESI Group - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16653 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16653
//
// <-- Short Description -->
//    fromJOSN(path, "file") crash on non exiting file
// =============================================================================

filename = "non-existing.json";
refMsg = msprintf(_("%s: Cannot open file %s.\n"), "fromJSON", filename);
assert_checkerror("fromJSON(filename, ""file"")", refMsg);
