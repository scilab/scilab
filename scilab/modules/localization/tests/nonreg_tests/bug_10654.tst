// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 10654 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10654
//
// <-- Short Description -->
// gettext with "scilab" domain did not return same result than gettext

A = gettext('scilab', '%s: No more memory.\n');
B = gettext("%s: No more memory.\n");
assert_checkequal(A, B);
