// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 14980 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14980
//
// <-- Short Description -->
// The datatip display of the root locus arcs is broken

H=syslin('c',352*poly(-5,'s')/poly([0,0,2000,200,25,1],'s','c'));
clf();evans(H,100);

//now add a datatip on ones of the locus arc