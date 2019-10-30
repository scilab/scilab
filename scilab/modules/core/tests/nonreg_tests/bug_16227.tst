// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 16227 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16227
//
// <-- Short Description -->
// WSCI was not available among environment variables

assert_checkequal(getenv("WSCI"), WSCI)

if getos()=="Windows" then
    assert_checktrue(dos("dir %WSCI%"))
else
    assert_checkequal(host("ls $WSCI"), 0)
end
