// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Jérôme Picard <jerome.picard@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
// <-- Non-regression test for bug 7214 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7214
//
// <-- Short Description -->
//    bug 7214 fixed - Modelica demos did not generate a .mo file in TMPDIR/modelica directory

// open a modelica demo : xcos --> Modelica demos --> Bouncing Ball

demopath = pathconvert(SCI+"/modules/xcos/demos/ModelicaBlocks/", %t, %t);
xcos(demopath + "BouncingBall_Modelica.zcos");

// simulate the diagram

// check if the modelica file exists in the TMPDIR/modelica directory
filepath = pathconvert(TMPDIR+"/modelica/", %t, %t);
if ~isfile(filepath + "Bounce.mo") then pause, end
