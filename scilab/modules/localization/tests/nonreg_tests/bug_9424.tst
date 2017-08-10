//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 9424 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9424
//
// <-- Short Description -->
// gettext("") did not return ""

assert_checkequal(gettext(""), "");
assert_checkequal(dgettext("scilab", ""), "");
assert_checkequal(dgettext("mydomain", ""), "");