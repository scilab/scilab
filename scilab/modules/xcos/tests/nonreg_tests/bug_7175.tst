// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 7175 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7175
//
// <-- Short Description -->
// demo bridge rectifier did not build on Windows.

// start demo --> eletrical systems --> bridge rectifier
// Open the initialization GUI.
// Try to solve the initialization problem (using the Kinsol method for example).
// demo must build
// previous error was :
// Bridge_Rectifier_imfi.obj : error LNK2019: unresolved external symbol 
// _read_xml_initial_states referenced in function _Bridge_Rectifier_imfilib
// Bridge_Rectifier_imfi.dll : fatal error LNK1120: 1 unresolved externals

