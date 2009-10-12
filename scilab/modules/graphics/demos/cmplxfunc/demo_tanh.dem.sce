//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// tanh
// =============================================================================

function demo_tanh()
	
	R     = %pi/2-0.2;
	e     = 0;
	theta = -130;
	alpha = 73;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,e,"Square","Ox",41,"tanh",theta,alpha,[-R,R]);
	
endfunction

demo_tanh()
demo_viewCode("demo_tanh.dem.sce");
clear demo_tanh;
