//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// exp
// =============================================================================

function demo_exp()
	
	R     = 2;
	theta = -130;
	alpha = 73;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,0,"Disk","Ox",[40 20],"exp",theta,alpha,[-R,R]);
	
endfunction

demo_exp()
demo_viewCode("demo_exp.dem.sce");
clear demo_exp;
