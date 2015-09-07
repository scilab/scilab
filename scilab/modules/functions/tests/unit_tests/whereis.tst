//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//whereis

assert_checkequal(whereis("sci2exp"), "stringlib");
assert_checkequal(whereis("aaa"), []);

assert_checkequal(whereis("with_tk"), "corelib");
a = corelib;
assert_checkequal(whereis("with_tk"), ["a" ; "corelib"]);


