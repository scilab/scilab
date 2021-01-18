// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Stephane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16631 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16631
//
// <-- Short Description -->
// read-only handle properties are considered as unknown


cmd = "gcf().type=""foo""";
message = msprintf(_("Read-only property: %s."),"type");
assert_checkerror(cmd, message)

