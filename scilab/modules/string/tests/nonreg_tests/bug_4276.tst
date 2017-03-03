// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 4276 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4276
//
// <-- Short Description -->
// strsubst replaced the first occurence in regex mode.

assert_checkequal(strsubst('abc012def', '/[a-z]+/', '', 'r'), "012");
assert_checkequal(strsubst('abc012def', '/[a-z]+/', 'xyz', 'r'), "xyz012xyz");
assert_checkequal(strsubst('abc012-def456', '/[a-z]+[0-9]+/', 'qrstuvwxyz', 'r'), "qrstuvwxyz-qrstuvwxyz");
