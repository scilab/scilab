//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// sinh
// =============================================================================

function demo_sinh()
	
	R     = %pi;
	e     = 0;
	theta = -148;
	alpha = 60;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,0,"Disk","Ox",[40 20],"sinh",theta,alpha,[-R,R]);
	
endfunction

demo_sinh()
demo_viewCode("demo_sinh.dem.sce");
clear demo_sinh;
