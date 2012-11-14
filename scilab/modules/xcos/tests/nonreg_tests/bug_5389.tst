/ =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 5389 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5389
//
// <-- Short Description -->
// if there was no C compiler detected on Windows, xcos/scicos tried to call dynamic link functions.

// 1] overload haveacompiler
// function r = haveacompiler()
//   r = %f;
// endfunction

// 2] start a xcos demos with modelica for example

// 3] you should have a message box with : 
// [_("Sorry compiling problem");_("A compatible C compiler required.")

