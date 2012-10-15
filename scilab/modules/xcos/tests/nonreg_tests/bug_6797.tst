// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6541 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6797
//
// <-- Short Description -->
// Calling importXcosDiagram without loading Scicos libs must not fail.
// The call of importXcosDiagram must not load scicos libs.


// Are scicos libs loaded ?
function result = isScicosLibLoaded()
	result = isdef('haltscicos');
endfunction

// Check that the simulation librairies are not loaded at startup
if isScicosLibLoaded() <> %f then pause,end

result = importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_6797.xcos");
if result <> %t then pause,end

// Check that the simulation librairies are not loaded after the call
if isScicosLibLoaded() <> %f then pause,end

