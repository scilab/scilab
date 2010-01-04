//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// atanh
// =============================================================================

function demo_atanh()
	
	R     = 2;
	e     = 0.001;
	theta = -110;
	alpha = 75;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,e,"Square","Ox",41,"atanh",theta,alpha,[-0.99,0.99]);
	
endfunction

demo_atanh()
demo_viewCode("demo_atanh.dem.sce");
clear demo_atanh;
