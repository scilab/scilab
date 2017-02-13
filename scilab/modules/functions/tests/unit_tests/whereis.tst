//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//whereis

assert_checkequal(whereis("aaa"), []);

//macros
assert_checkequal(whereis("sci2exp"), "stringlib");
assert_checkequal(whereis(sci2exp), "stringlib");
assert_checkequal(whereis("with_tk"), "corelib");
assert_checkequal(whereis(with_tk), "corelib");

a = corelib;
assert_checkequal(whereis("with_tk"), ["a" ; "corelib"]);

//builtin
assert_checkequal(whereis("exit"), "core");
assert_checkequal(whereis(exit), "core");
assert_checkequal(whereis("length"), "string");
assert_checkequal(whereis(length), "string");
