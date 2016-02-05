// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 DIGITEO Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->
//
// <-- Short Description -->
// This script validate the eselect functionality with a sinus input.
// The associated diagram (sinus.zcos) is run and export data to the workspace.
// These data are then compared to the default.
//

currentPath = SCI + "/modules/xcos/tests/unit_tests/block_tests/ESELECT_f/";

// Import the reference data (A_ref)
ierr = execstr("load(currentPath + ""sinus_ref.sod"");", "errcatch");
assert_checktrue(ierr == 0);

// Import the diagram structure
ierr = importXcosDiagram(currentPath + "sinus.zcos");
if ierr <> %t then pause; end

// launch the simulation
scicos_simulate(scs_m);
assert_checkalmostequal(A_ref("values"), A("values"));
