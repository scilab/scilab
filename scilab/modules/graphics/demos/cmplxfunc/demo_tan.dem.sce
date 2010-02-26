//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// tan
// =============================================================================

function demo_tan()
	
	R     = %pi/2-0.15;
	theta = -130;
	alpha = 73;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,0,"Square","Ox",41,"tan",theta,alpha,[-R,R]);
	
endfunction

demo_tan()
demo_viewCode("demo_tan.dem.sce");
clear demo_tan;
