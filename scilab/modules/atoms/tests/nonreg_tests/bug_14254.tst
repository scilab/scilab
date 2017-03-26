// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 14254 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14254
//
// <-- Short Description -->
// When installing an ATOMS module that is not locally registered and available,
// the error message was unclear

instruction = "atomsInstall(""xfgyzk_nothing"");";
assert_checktrue(execstr(instruction, 'errcatch')~=0);

expectedMsg = "%s: The package ""%s"" is not registered.\nPlease check on the ATOMS repository that it is available for Scilab %d.%d on %s.\nIf it is, run atomsSystemUpdate() before trying atomsInstall(..) again.\n\n";
Vers = getversion('scilab');
expectedMsg = msprintf(expectedMsg, "atomsInstallList", "xfgyzk_nothing", Vers(1:2), getos());
//assert_checkerror(instruction, expectedMsg);
