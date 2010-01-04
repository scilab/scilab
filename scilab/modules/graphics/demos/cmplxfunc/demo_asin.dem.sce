//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// asin
// =============================================================================

function demo_asin()
	
	theta = -110;
	alpha = 75;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(2,%eps,"Square","Ox",41,"asin",theta,alpha,[-1,1]);
	
endfunction

demo_asin()
demo_viewCode("demo_asin.dem.sce");
clear demo_asin;
