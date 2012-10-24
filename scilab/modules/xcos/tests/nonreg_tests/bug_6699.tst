// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 6699 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6699
//
// <-- Short Description -->
// The capacitor block doesn't define valid settings.

scs_m = Capacitor("define", [], []);

// Stubbing the x_mdialog method
funcprot(0);
// checking it's arguments only
function [result]=x_mdialog(title,labels,default_inputs_vector)

	if(and(size(labels) <> size(default_inputs_vector))) then pause, end;
	
	result = default_inputs_vector;
endfunction

scs_m = Capacitor("set", scs_m, []);

