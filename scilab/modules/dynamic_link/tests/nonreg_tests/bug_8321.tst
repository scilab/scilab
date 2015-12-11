// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8321 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8321
//
// <-- Short Description -->
// configure_msvc failed and returned a wrong error message
// =============================================================================

// In a special case: visual studio installed and removed.
// registry key used to detect visual studio are not removed by visual studio.
// In this case, path returned by registry key is invalid and configuration returned a wrong
// message.
// This trouble is NOT easy to test
// Install Visual studio
// Uninstall Visual studio
// start scilab and check that you do not have this error message:
 
//Startup execution:
//  loading initial environment
// !--error 999 
//setenv: Wrong type for input argument #2: string expected.
//at line       6 of function dlwSetEnvCommonVc9Vc10 called by :  

// You must have:
warning(msprintf(gettext('Microsoft Visual Studio C x86 Compiler not found.')));
