//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// acosh
// =============================================================================

function demo_acosh()
	
	R     = 2;
	e     = %eps;
	theta = -110;
	alpha = 75;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,e,"Square","Ox",41,"acosh",theta,alpha,[1,R]);
	
endfunction

demo_acosh()
demo_viewCode("demo_acosh.dem.sce");
clear demo_acosh;
