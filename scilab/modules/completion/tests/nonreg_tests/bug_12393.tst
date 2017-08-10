//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JAVA NOT MANDATORY -->
//
// <-- Non-regression test for bug 10393 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12393
//
// <-- Short Description -->
// During completion replacment by common part of suggested words does not work

ilib_verbose(0);
ierr = exec(SCI + "/modules/completion/tests/utilities/build_primitives.sce", ..
    "errcatch", -1);
assert_checkequal(ierr, 0);
ierr = exec(TMPDIR + "/completion/loader.sce", "errcatch", -1);
assert_checkequal(ierr, 0);

// Suggested words for 'setdefau' are 'setdefaultlanguage', 'setDefaultColor'
// We must replace 'setdefau' by common part of this words: 'set'
currentline = 'setdefau';
r = completeline('setdefau', 'set', getfilepartlevel(currentline), ..
    getpartlevel(currentline), %t);
assert_checkequal(r, 'set');
