//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// cosh
// =============================================================================

function demo_cosh()
	
	R     = %pi;
	e     = 0;
	theta = -130;
	alpha = 56;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,e,"Disk","Ox",[40 20],"cosh",theta,alpha,[-R,R]);
	
endfunction

demo_cosh()
demo_viewCode("demo_cosh.dem.sce");
clear demo_cosh;
