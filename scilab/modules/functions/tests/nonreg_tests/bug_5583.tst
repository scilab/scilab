// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5583 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5583
//
// <-- Short Description -->
// 'exec' did not manage correctly UTF-8 and ANSI files encoded.

exec SCI/modules/functions/tests/nonreg_tests/bug_5583_ANSI.sce;
exec SCI/modules/functions/tests/nonreg_tests/bug_5583_UTF.sce;
