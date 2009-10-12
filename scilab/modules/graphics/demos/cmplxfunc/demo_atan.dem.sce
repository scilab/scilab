//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// atan
// =============================================================================

function demo_atan()
	
	R     = 2;
	theta = -110;
	alpha = 75;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,0.001,"Square","Oy",41,"atan",theta,alpha,[-R,R]);
	
endfunction

demo_atan()
demo_viewCode("demo_atan.dem.sce");
clear demo_atan;
