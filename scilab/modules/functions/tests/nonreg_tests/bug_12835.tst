// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12835 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12835
//
// <-- Short Description -->
// When in a function returning varargout varargout is set to an empty list(),
// calling it yielded an error.

deff('varargout = test()','varargout=list()');
assert_checkequal(execstr("test()", "errcatch"), 0);
