// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 8628 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8628
//
// <-- Short Description -->
// Permanent variable check.

refMsg = msprintf(_("Redefining permanent variable.\n"));

assert_checkerror( "editvar(''%e'')",       refMsg);
assert_checkerror( "editvar(''%eps'')",		refMsg);
assert_checkerror( "editvar(''%fftw'')",	refMsg);
assert_checkerror( "editvar(''%gui'')",		refMsg);
assert_checkerror( "editvar(''%i'')",		refMsg);
assert_checkerror( "editvar(''%io'')",		refMsg);
assert_checkerror( "editvar(''%inf'')",		refMsg);
assert_checkerror( "editvar(''%nan'')",		refMsg);
assert_checkerror( "editvar(''%pi'')",		refMsg);
assert_checkerror( "editvar(''%s'')",		refMsg);
assert_checkerror( "editvar(''%tk'')",		refMsg);
assert_checkerror( "editvar(''%z'')",		refMsg);
assert_checkerror( "editvar(''evoid'')",	refMsg);
assert_checkerror( "editvar(''home'')",		refMsg);
assert_checkerror( "editvar(''PWD'')",		refMsg);
assert_checkerror( "editvar(''SCI'')",		refMsg);
assert_checkerror( "editvar(''SCIHOME'')",	refMsg);
assert_checkerror( "editvar(''TMPDIR'')",	refMsg);
