// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - ESI Group - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16849 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16849
//
// <-- Short Description -->
//    toJSON("\\") must return "\\\\"
// =============================================================================

assert_checkequal(toJSON("\"), """\\""");
assert_checkequal(toJSON("\\"), """\\\\""");
