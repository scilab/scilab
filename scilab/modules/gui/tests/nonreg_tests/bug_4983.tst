// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 4983 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4983
//
// <-- Short Description -->
// when scilab is started in nw mode, the addmenu examples are failing with a wierd error message.

// launch scilab in NW or NWNI mode
  foo = 'disp(''hello'')';
  addmenu('foo');
  
// it must return a error as:
// addmenu: can not add a menu in console in this mode: NW.