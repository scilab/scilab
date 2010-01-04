//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// cos
// =============================================================================

function demo_cos()
	
	R     = %pi;
	theta = 18;
	alpha = 43;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,0,"Disk","Ox",[40 20],"cos",theta,alpha,[-R,R]);
	
endfunction

demo_cos()
demo_viewCode("demo_cos.dem.sce");
clear demo_cos;
