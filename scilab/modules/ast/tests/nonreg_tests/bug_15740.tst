// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15740 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15740
//
// <-- Short Description -->
// %s == %z now returns %T and %s ~= %z returns %F  (Regressions)

assert_checkfalse(%s == %z)
assert_checktrue(%s ~= %z)