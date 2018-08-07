// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI-Group - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14915 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14915
//
// <-- Short Description -->
// ascii(10) makes mprintf() stop printing.

assert_checkequal(msprintf("bug_14915"), "bug_14915");
assert_checkequal(msprintf("bug_14915%s", ascii(10)), "bug_14915"+ascii(10));
assert_checkequal(msprintf("%sbug_14915", ascii(10)), ascii(10)+"bug_14915");
assert_checkequal(msprintf("%sbug_14915%s", ascii(10), ascii(10)), ascii(10)+"bug_14915"+ascii(10));
assert_checkequal(msprintf("bug\n14915"), ["bug"; "14915"]);
assert_checkequal(msprintf("test\nbug%d14\n915", [1 2 3]'), ["test"; "bug114"; "915test"; "bug214"; "915test"; "bug314"; "915"]);
assert_checkequal(msprintf("test\nbug%d14\n915\n", [1 2 3]'), ["test"; "bug114"; "915"; "test"; "bug214"; "915"; "test"; "bug314"; "915"]);
assert_checkequal(msprintf("bug%s14915\n", ["_"+ascii(10)+"_" "_" "_\n_"]'), ["bug_"+ascii(10)+"_14915"; "bug_14915"; "bug_\n_14915"]);
assert_checkequal(msprintf("bug%s14915", ["_"+ascii(10)+"_" "_" "_\n_"]'), ["bug_"+ascii(10)+"_14915bug_14915bug_\n_14915"]);