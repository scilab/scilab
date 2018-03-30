// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//<-- CLI SHELL MODE -->
//<-- NO CHECK REF -->
//
// <-- Non-regression test for bug 9063 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9063
//
// <-- Short Description -->
// "pt" was the alias of "pt_PT" instead of "pt_BR"

setlanguage("pt")
assert_checkequal(getlanguage(), "pt_BR");
