// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 14992 -->

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14992

// <-- Short Description -->
// readgateway function was removeed, use whereis instead

assert_checkequal(whereis("cos"), "elementary_functions");
assert_checkequal(whereis(cos), "elementary_functions");
assert_checkequal(whereis("acosd"), "elementary_functionslib");
assert_checkequal(whereis(acosd), "elementary_functionslib");
