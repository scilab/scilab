//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 1805 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/1805
//
// <-- Short Description -->
//    varargout must be a list. It is initialized to list() by default

funcprot(0);

function varargout = test(), varargout = ""; a = 1, endfunction
assert_checkfalse(execstr("test()","errcatch")==0);

function varargout = test(); a = 1, endfunction
assert_checkequal(execstr("test()","errcatch"), 0);
