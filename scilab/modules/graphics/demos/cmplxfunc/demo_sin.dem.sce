//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// sin
// =============================================================================

function demo_sin()
	
	R     = %pi;
	theta = -130;
	alpha = 73;
	e     = 0.001;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,e,"Disk","Ox",[40 20],"sin",theta,alpha,[-R,R]);
	
endfunction

demo_sin()
demo_viewCode("demo_sin.dem.sce");
clear demo_sin;
