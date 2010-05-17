//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// asinh
// =============================================================================

function demo_asinh()
	
	R     = 2;
	e     = %eps;
	theta = -110;
	alpha = 75;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,e,"Square","Oy",41,"asinh",theta,alpha,[-R,R]);
	
endfunction

demo_asinh()
demo_viewCode("demo_asinh.dem.sce");
clear demo_asinh;
